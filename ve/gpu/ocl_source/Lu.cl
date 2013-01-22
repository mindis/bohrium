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
#define BLOCK_SIZE 32

    
    __kernel void pivot(__global float *A, long k, __global int* piv, long n){
      
      int tid = get_local_id(0);
      
      float max = 0;
      int p = 0;
      
      for(int i = tid; i < n; i+=get_local_size(0)){
        if( i >= k && fabs(A[i*n + k]) > max){
          max = fabs(A[i * n +k]);
          p = i;
        }
      }
      
      
      __local float local_max[256];
      __local int local_p[256];
      
      local_max[tid] = max;
      local_p[tid] = p;
      
      barrier(CLK_LOCAL_MEM_FENCE);
      
      if(tid == 0){
        for(int i = 0; i < 256; i++){
          if( fabs(local_max[i]) > max){
            max = fabs(local_max[i]);
            p = local_p[i];
          }          
        }
        piv[k] = p;
      }
      barrier(CLK_GLOBAL_MEM_FENCE);
      
      //I both find, and pivot in one work-group, as it is cheaper than invoking several kernels
      for(int i = tid; i < n; i+=get_local_size(0)){
         float temp = A[k*n + i];
         A[k*n + i] = A[piv[k]*n + i];
         A[piv[k]*n + i] = temp;
      }
    } 
    
    __kernel void update_col(__global float *A, long k, long n){
      unsigned int row = get_global_id(0);
      if(row > k && row < n)
        A[row*n+k] = A[row*n+k] / A[n*k+k];
    }
    __kernel void update_rest(__global float *A, long k, long n){
        unsigned int row = get_global_id(0);
        unsigned int col = get_global_id(1);
        
        if(col > k && row > k && col < n && row < n){
            A[row*n+col] = A[row*n+col] - A[row * n + k] * A[k * n + col];          
        }    
    }
    

    __kernel void update_block_col(__global float *A, long k, long block_start, long n){
       
       unsigned int row = get_global_id(0) + block_start;
       unsigned int col = get_global_id(1) + block_start;
       
       if(row > k && col > k){
         A[row*n+col] = A[row*n+col] - A[row * n + k] * A[k * n + col];
       }
    }
    
    //strsm
    __kernel void update_block_row(__global float *A, long L_start_k, long n){
       
       unsigned int col = get_global_id(0);
       
       __global float *L = &A[L_start_k * n + L_start_k];
       A = &A[L_start_k * n + L_start_k + BLOCK_SIZE];
       
       
       for(int r = 0; r < BLOCK_SIZE; r++){
         float u0 = 0;
         
         for(int k = 0; k < r; k++){
           u0 -= L[k + r * n]*A[col + k * n];
         }
         A[col + r * n] = A[col + r * n] + u0;
       }      
    }

    
    __kernel void mm(__global float* C, __global const float* A, __global const float* B, long a_width, long row_lda, long k){
    unsigned int local_col = get_local_id(0);
    unsigned int local_row = get_local_id(1);
    unsigned int col = get_global_id(0);
    unsigned int row = get_global_id(1);
    __local float A_cache[BLOCK_SIZE*BLOCK_SIZE];
    __local float B_cache[BLOCK_SIZE*BLOCK_SIZE];
    
    float val = 0;
    
    //in lu this loop should always run only one time
    for(int k0 = 0; k0 < a_width; k0 += BLOCK_SIZE){
      A_cache[local_col + local_row*BLOCK_SIZE] = A[(k+BLOCK_SIZE + row) * row_lda +  (k0 + k + local_col)];
      B_cache[local_col + local_row*BLOCK_SIZE] = B[(k0+k+local_row) * row_lda + (col + k + BLOCK_SIZE)];
      
      barrier(CLK_LOCAL_MEM_FENCE);
      
      for(int k = 0; k < BLOCK_SIZE; k++){
        val += A_cache[k+local_row*BLOCK_SIZE] * B_cache[local_col+k*BLOCK_SIZE];       
      }
      barrier(CLK_LOCAL_MEM_FENCE);     
      
    }
    C[(col+k+BLOCK_SIZE) + (row+k+BLOCK_SIZE)*row_lda] -= val;
    
    
    }
    
    
