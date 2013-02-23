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

#ifndef __USERFUNCTIONMATMUL_HPP
#define __USERFUNCTIONMATMUL_HPP

#include <string>
#include <map>
#include <bh.h>
#include "UserFuncArg.hpp"
#include "Kernel.hpp"
#include "StringHasher.hpp"

namespace UserFunctionMatmul
{
    typedef std::map<size_t, Kernel> KernelMap;
    static KernelMap kernelMap;
    void matmul(bh_matmul_type* matmulDef, UserFuncArg* userFuncArg);
    Kernel getKernel(bh_matmul_type* matmulDef,
                     UserFuncArg* userFuncArg);
    std::string generateCode(bh_matmul_type* matmulDef,
                             OCLtype type);
    std::string generateDefines(bh_matmul_type* matmulDef,
                             OCLtype type);
}

#endif