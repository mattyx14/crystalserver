//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef ___MD5_H___
#define ___MD5_H___

#include "definitions.h"

/* Typedef a 32 bit type */
#ifndef UINT4
typedef uint32_t UINT4;
#endif

/* Data structure for MD5 (Message Digest) computation */
typedef struct {
	UINT4 i[2];                   /* Number of _bits_ handled mod 2^64 */
	UINT4 buf[4];                                    /* Scratch buffer */
	unsigned char in[64];                              /* Input buffer */
	unsigned char digest[16];     /* Actual digest after MD5Final call */
} MD5_CTX;

void MD5_Transform (UINT4 *buf, UINT4 *in);

void MD5Init(MD5_CTX *mdContext, unsigned long pseudoRandomNumber = 0);
void MD5Update(MD5_CTX *mdContext, const unsigned char *inBuf, unsigned int inLen);
void MD5Final(MD5_CTX *mdContext);

#endif /* ___MD5_H___ included */
