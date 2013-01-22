/*
This file is part of Bohrium and copyright (c) 2012 the Bohrium
team <http://www.bh107.org>.

Bohrium is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as 
published by the Free Software Foundation, either version 3 
of the License, or (at your option) any later version.

Bohrium is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the 
GNU Lesser General Public License along with Bohrium. 

If not, see <http://www.gnu.org/licenses/>.
*/
#include <bh_vcache.h>
//
// C-Friendly version of the vcache.
// 
static bh_data_ptr   *bh_vcache;
static bh_intp       *bh_vcache_bytes;
static bh_intp   bh_vcache_bytes_total;
static int          bh_vcache_size;
static int          bh_vcache_cur;

static int bh_vcache_hits = 0;
static int bh_vcache_miss = 0;
static int bh_vcache_store = 0;
static int bh_vcache_flush = 0;

/**
 *  Reset vcache counters
 */
inline
void bh_vcache_reset_counters() {
    bh_vcache_hits = 0;
    bh_vcache_miss = 0;
    bh_vcache_store = 0;
    bh_vcache_flush = 0;
}

/**
 *  Initiate vcache to a fixed size.
 *
 *  cache will hold 'size' elements in the cache.
 *
 *  Expiration / cache invalidation is based on round-robin;
 *  Whenever an element is added to vcache the round-robin
 *  counter is incremented.
 *
 */
void bh_vcache_init( int size )
{
    bh_vcache_size           = size;
    bh_vcache_bytes_total    = 0;

    bh_vcache_cur    = 0;
    bh_vcache        = (bh_data_ptr*)malloc(sizeof(bh_data_ptr)*size);
    memset(bh_vcache,0,sizeof(bh_data_ptr)*size);
    bh_vcache_bytes  = (bh_intp*)malloc(sizeof(bh_intp)*size);
    memset(bh_vcache_bytes,0,sizeof(bh_intp)*size);

    bh_vcache_reset_counters();
}

/**
 * Deallocates vcache arrays.
 */
void bh_vcache_delete()
{
    free( bh_vcache );
    free( bh_vcache_bytes );
}

/**
 * Remove all entries from vcache and de-allocate them
 */
void bh_vcache_clear()
{
    int i;
    for (i=0; i<bh_vcache_size; i++) {
        if (bh_vcache_bytes[i] > 0) {
            bh_memory_free( bh_vcache[i], bh_vcache_bytes[i] );
        }
    }
}

/**
 * Return and remove a pointer of size 'size' from vcache.
 *
 * This removes it from the vcache!
 *
 * @return null If none exists.
 */
bh_data_ptr bh_vcache_find( bh_intp bytes )
{
    int i;
    for (i=0; i<bh_vcache_size; i++) {
        if (bh_vcache_bytes[i] == bytes) {
            DEBUG_PRINT("Found %p in vcache.\n", bh_vcache[i]);
            bh_vcache_hits++;
            bh_vcache_bytes[i] = 0;
            bh_vcache_bytes_total -= bytes;
            return bh_vcache[i];
        }
    }
    bh_vcache_miss++;
    return NULL;
}

/**
 * Add an element to vcache.
 * 
 * @param data Pointer to allocated data.
 * @param size Size in bytes of the allocated data.
 */
void bh_vcache_insert( bh_data_ptr data, bh_intp size )
{
    if (bh_vcache_bytes[bh_vcache_cur] > 0) {
		DEBUG_PRINT("Free=%p\n", bh_vcache[bh_vcache_cur]);
        bh_memory_free( bh_vcache[bh_vcache_cur], bh_vcache_bytes[bh_vcache_cur] );
    }

    bh_vcache[bh_vcache_cur] = data;
    bh_vcache_bytes[bh_vcache_cur] = size;

    bh_vcache_cur = (bh_vcache_cur+1) % bh_vcache_size;
    bh_vcache_bytes_total += size;

    bh_vcache_store++;
}

/**
 * De-allocate memory for an instructions output operand.
 *
 */
bh_error bh_vcache_free( bh_instruction* inst )
{
    bh_array* base;
    bh_intp nelements, bytes;

    base = bh_base_array( inst->operand[0] ); 
    if (base->data != NULL) {
        nelements   = bh_nelements(base->ndim, base->shape);
        bytes       = nelements * bh_type_size(base->type);
        
		DEBUG_PRINT("Deallocate=%p\n", base->data);
        bh_vcache_insert( base->data, bytes );
        bh_vcache_store++;
		base->data = NULL;
    }
    inst->operand[0] = NULL;

    return BH_SUCCESS;
}

/**
 * Allocate memory for the output operand of the given instruction.
 * A previous (now unused) memory allocation will be assigned if available.
 *
 */
bh_error bh_vcache_malloc( bh_instruction* inst )
{
    bh_array* base;
    bh_intp nops, i, nelements, bytes;

    switch (inst->opcode) {                     // Allocate memory for built-in

        case BH_NONE:                        // No memory operations for these
        case BH_DISCARD:
        case BH_SYNC:
        case BH_USERFUNC:
        case BH_FREE:
            break;

        default:                                    

            nops = bh_operands(inst->opcode);    // Allocate memory for operands        
            for(i=0; i<nops; i++)
            {
                if (!bh_is_constant(inst->operand[i]))
                {
                    //We allow allocation of the output operand only
                    if (i == 0)
                    {
                        base = bh_base_array( inst->operand[0] );    // Reuse or allocate
                        if (base->data == NULL) {

                            nelements   = bh_nelements(base->ndim, base->shape);
                            bytes       = nelements * bh_type_size(base->type);

                            DEBUG_PRINT("Reuse or allocate...\n");
                            base->data = bh_vcache_find( bytes );
                            if (base->data == NULL) {
                                if (bh_data_malloc(inst->operand[0]) != BH_SUCCESS) {
                                    inst->status = BH_OUT_OF_MEMORY;
                                    return BH_OUT_OF_MEMORY;         // EXIT
                                }                                   
                                DEBUG_PRINT("Allocated=%p\n", base->data);
                                bh_vcache_miss++;
                            } else {
                                DEBUG_PRINT("Reusing=%p.\n", base->data);
                                bh_vcache_hits++;
                            }
                        }
                    }
                    else if(bh_base_array(inst->operand[i])->data == NULL) 
                    {
                        inst->status = BH_ERROR;
                        return BH_ERROR; // EXIT
                    }
                }

            }
            break;

    }

    return BH_SUCCESS;

}
