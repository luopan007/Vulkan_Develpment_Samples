#ifndef __OpenGL3_0Demo__Matrix__
#define __OpenGL3_0Demo__Matrix__
#include <math.h>
#include <assert.h>
class Matrix {
public:
    static void multiplyMM(float* result, int resultOffset, float* mlIn, int lhsOffset, float* mrIn, int rhsOffset)
    {
        double ml[16];
		double mr[16];
		for(int i=0;i<16;i++)
		{
			ml[i]=mlIn[i];
			mr[i]=mrIn[i];
		}
		result[0+resultOffset]=(float) (ml[0+lhsOffset]*mr[0+rhsOffset]+ml[4+lhsOffset]*mr[1+rhsOffset]+ml[8+lhsOffset]*mr[2+rhsOffset]+ml[12+lhsOffset]*mr[3+rhsOffset]);
		result[1+resultOffset]=(float) (ml[1+lhsOffset]*mr[0+rhsOffset]+ml[5+lhsOffset]*mr[1+rhsOffset]+ml[9+lhsOffset]*mr[2+rhsOffset]+ml[13+lhsOffset]*mr[3+rhsOffset]);
		result[2+resultOffset]=(float) (ml[2+lhsOffset]*mr[0+rhsOffset]+ml[6+lhsOffset]*mr[1+rhsOffset]+ml[10+lhsOffset]*mr[2+rhsOffset]+ml[14+lhsOffset]*mr[3+rhsOffset]);
		result[3+resultOffset]=(float) (ml[3+lhsOffset]*mr[0+rhsOffset]+ml[7+lhsOffset]*mr[1+rhsOffset]+ml[11+lhsOffset]*mr[2+rhsOffset]+ml[15+lhsOffset]*mr[3+rhsOffset]);
		result[4+resultOffset]=(float) (ml[0+lhsOffset]*mr[4+rhsOffset]+ml[4+lhsOffset]*mr[5+rhsOffset]+ml[8+lhsOffset]*mr[6+rhsOffset]+ml[12+lhsOffset]*mr[7+rhsOffset]);
		result[5+resultOffset]=(float) (ml[1+lhsOffset]*mr[4+rhsOffset]+ml[5+lhsOffset]*mr[5+rhsOffset]+ml[9+lhsOffset]*mr[6+rhsOffset]+ml[13+lhsOffset]*mr[7+rhsOffset]);
		result[6+resultOffset]=(float) (ml[2+lhsOffset]*mr[4+rhsOffset]+ml[6+lhsOffset]*mr[5+rhsOffset]+ml[10+lhsOffset]*mr[6+rhsOffset]+ml[14+lhsOffset]*mr[7+rhsOffset]);
		result[7+resultOffset]=(float) (ml[3+lhsOffset]*mr[4+rhsOffset]+ml[7+lhsOffset]*mr[5+rhsOffset]+ml[11+lhsOffset]*mr[6+rhsOffset]+ml[15+lhsOffset]*mr[7+rhsOffset]);
		result[8+resultOffset]=(float) (ml[0+lhsOffset]*mr[8+rhsOffset]+ml[4+lhsOffset]*mr[9+rhsOffset]+ml[8+lhsOffset]*mr[10+rhsOffset]+ml[12+lhsOffset]*mr[11+rhsOffset]);
		result[9+resultOffset]=(float) (ml[1+lhsOffset]*mr[8+rhsOffset]+ml[5+lhsOffset]*mr[9+rhsOffset]+ml[9+lhsOffset]*mr[10+rhsOffset]+ml[13+lhsOffset]*mr[11+rhsOffset]);
		result[10+resultOffset]=(float) (ml[2+lhsOffset]*mr[8+rhsOffset]+ml[6+lhsOffset]*mr[9+rhsOffset]+ml[10+lhsOffset]*mr[10+rhsOffset]+ml[14+lhsOffset]*mr[11+rhsOffset]);
		result[11+resultOffset]=(float) (ml[3+lhsOffset]*mr[8+rhsOffset]+ml[7+lhsOffset]*mr[9+rhsOffset]+ml[11+lhsOffset]*mr[10+rhsOffset]+ml[15+lhsOffset]*mr[11+rhsOffset]);
		result[12+resultOffset]=(float) (ml[0+lhsOffset]*mr[12+rhsOffset]+ml[4+lhsOffset]*mr[13+rhsOffset]+ml[8+lhsOffset]*mr[14+rhsOffset]+ml[12+lhsOffset]*mr[15+rhsOffset]);
		result[13+resultOffset]=(float) (ml[1+lhsOffset]*mr[12+rhsOffset]+ml[5+lhsOffset]*mr[13+rhsOffset]+ml[9+lhsOffset]*mr[14+rhsOffset]+ml[13+lhsOffset]*mr[15+rhsOffset]);
		result[14+resultOffset]=(float) (ml[2+lhsOffset]*mr[12+rhsOffset]+ml[6+lhsOffset]*mr[13+rhsOffset]+ml[10+lhsOffset]*mr[14+rhsOffset]+ml[14+lhsOffset]*mr[15+rhsOffset]);
		result[15+resultOffset]=(float) (ml[3+lhsOffset]*mr[12+rhsOffset]+ml[7+lhsOffset]*mr[13+rhsOffset]+ml[11+lhsOffset]*mr[14+rhsOffset]+ml[15+lhsOffset]*mr[15+rhsOffset]);
    }
    static void multiplyMV (float* resultVec, int resultVecOffset, float* mlIn, int lhsMatOffset,
                            float* vrIn, int rhsVecOffset)
	{
		double ml[16];
		double vr[4];
		for(int i=0;i<16;i++)
		{
			ml[i]=mlIn[i];
		}
		vr[0]=vrIn[0];
		vr[1]=vrIn[1];
		vr[2]=vrIn[2];
		vr[3]=vrIn[3];
		resultVec[0+resultVecOffset]=(float) (ml[0+lhsMatOffset]*vr[0+rhsVecOffset]+ml[4+lhsMatOffset]*vr[1+rhsVecOffset]+ml[8+lhsMatOffset]*vr[2+rhsVecOffset]+ml[12+lhsMatOffset]*vr[3+rhsVecOffset]);
		resultVec[1+resultVecOffset]=(float) (ml[1+lhsMatOffset]*vr[0+rhsVecOffset]+ml[5+lhsMatOffset]*vr[1+rhsVecOffset]+ml[9+lhsMatOffset]*vr[2+rhsVecOffset]+ml[13+lhsMatOffset]*vr[3+rhsVecOffset]);
		resultVec[2+resultVecOffset]=(float) (ml[2+lhsMatOffset]*vr[0+rhsVecOffset]+ml[6+lhsMatOffset]*vr[1+rhsVecOffset]+ml[10+lhsMatOffset]*vr[2+rhsVecOffset]+ml[14+lhsMatOffset]*vr[3+rhsVecOffset]);
		resultVec[3+resultVecOffset]=(float) (ml[3+lhsMatOffset]*vr[0+rhsVecOffset]+ml[7+lhsMatOffset]*vr[1+rhsVecOffset]+ml[11+lhsMatOffset]*vr[2+rhsVecOffset]+ml[15+lhsMatOffset]*vr[3+rhsVecOffset]);
	}
    static void setIdentityM (float* sm, int smOffset)
	{
		for(int i=0;i<16;i++)
		{
			sm[i]=0;
		}
		sm[0]=1;
		sm[5]=1;
		sm[10]=1;
		sm[15]=1;
	}
	static void translateM(float* m, int mOffset,float x, float y, float z)
	{
		for (int i=0 ; i<4 ; i++)
		{
			int mi = mOffset + i;
			m[12 + mi] += m[mi] * x + m[4 + mi] * y + m[8 + mi] * z;
		}
	}
    static void rotateM(float* m, int mOffset,float a, float x, float y, float z)
	{
		float rm[16];
		setRotateM(rm, 0, a, x, y, z);
		float rem[16];
		multiplyMM(rem, 0, m, 0, rm, 0);
		for(int i=0;i<16;i++)
		{
			m[i]=rem[i];
		}
	}
    static void setRotateM(float* m, int mOffset,float a, float x, float y, float z)
    {
        float radians = a * 3.14159f / 180.0f;
        float s = sin(radians);
        float c = cos(radians);
        float sm[16];
        setIdentityM(sm, 0);
        sm[0] = c + (1 - c) * x * x;
        sm[1] = (1 - c) * x * y - z * s;
        sm[2] = (1 - c) * x * z + y * s;
        sm[4] = (1 - c) * x * y + z * s;
        sm[5] = c + (1 - c) * y * y;
        sm[6] = (1 - c) * y * z - x * s;
        sm[8] = (1 - c) * x * z - y * s;
        sm[9] = (1 - c) * y * z + x * s;
        sm[10] = c + (1 - c) * z * z;
        for(int i=0;i<16;i++)
		{
			m[i]=sm[i];
		}
    }
    static void scaleM(float* m, int mOffset, float x, float y, float z)
    {
        float sm[16];
        setIdentityM(sm, 0);
        sm[0] = x;
        sm[5] = y;
        sm[10] = z;
        sm[15] = 1;
        float tm[16];
        multiplyMM(tm,0,m,0,sm,0);
        for(int i=0;i<16;i++)
		{
			m[i]=tm[i];
		}
    }
    static void transposeM(float* mTrans, int mTransOffset, float* m, int mOffset)
    {
        for (int i = 0; i < 4; i++)
        {
            int mBase = i * 4 + mOffset;
            mTrans[i + mTransOffset] = m[mBase];
            mTrans[i + 4 + mTransOffset] = m[mBase + 1];
            mTrans[i + 8 + mTransOffset] = m[mBase + 2];
            mTrans[i + 12 + mTransOffset] = m[mBase + 3];  
        }
    }
    static void frustumM(float* m, int offset, float left, float right, float bottom, float top, float near, float far)
    {
        const float r_width  = 1.0f / (right - left);
        const float r_height = 1.0f / (top - bottom);
        const float r_depth  = 1.0f / (near - far);
        const float x = 2.0f * (near * r_width);
        const float y = 2.0f * (near * r_height);
        const float A = 2.0f * ((right + left) * r_width);
        const float B = (top + bottom) * r_height;
        const float C = (far + near) * r_depth;
        const float D = 2.0f * (far * near * r_depth);
        m[offset + 0] = x;
        m[offset + 5] = y;
        m[offset + 8] = A;
        m[offset +  9] = B;
        m[offset + 10] = C;
        m[offset + 14] = D;
        m[offset + 11] = -1.0f;
        m[offset +  1] = 0.0f;
        m[offset +  2] = 0.0f;
        m[offset +  3] = 0.0f;
        m[offset +  4] = 0.0f;
        m[offset +  6] = 0.0f;
        m[offset +  7] = 0.0f;
        m[offset + 12] = 0.0f;
        m[offset + 13] = 0.0f;
        m[offset + 15] = 0.0f;
    }
    static void orthoM(float * m, int mOffset, float left, float right, float bottom, float top, float near, float far)
    {
        assert(left != right);
        assert(bottom != top);
        assert(near != far);
        float r_width  = 1.0f / (right - left);
        float r_height = 1.0f / (top - bottom);
        float r_depth  = 1.0f / (far - near);
        float x =  2.0f * (r_width);
        float y =  2.0f * (r_height);
        float z = -2.0f * (r_depth);
        float tx = -(right + left) * r_width;
        float ty = -(top + bottom) * r_height;
        float tz = -(far + near) * r_depth;
        m[mOffset + 0] = x;
        m[mOffset + 5] = y;
        m[mOffset +10] = z;
        m[mOffset +12] = tx;
        m[mOffset +13] = ty;
        m[mOffset +14] = tz;
        m[mOffset +15] = 1.0f;
        m[mOffset + 1] = 0.0f;
        m[mOffset + 2] = 0.0f;
        m[mOffset + 3] = 0.0f;
        m[mOffset + 4] = 0.0f;
        m[mOffset + 6] = 0.0f;
        m[mOffset + 7] = 0.0f;
        m[mOffset + 8] = 0.0f;
        m[mOffset + 9] = 0.0f;
        m[mOffset + 11] = 0.0f;
    }
    static void setLookAtM(float* rm, int rmOffset, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
    {
        float fx = centerX - eyeX;
        float fy = centerY - eyeY;
        float fz = centerZ - eyeZ;
        float rlf = 1.0f /sqrt(fx*fx + fy*fy +fz*fz);
        fx *= rlf;
        fy *= rlf;
        fz *= rlf;
        float sx = fy * upZ - fz * upY;
        float sy = fz * upX - fx * upZ;
        float sz = fx * upY - fy * upX;
        float rls = 1.0f /sqrt(sx*sx + sy*sy +sz*sz);
        sx *= rls;
        sy *= rls;
        sz *= rls;
        float ux = sy * fz - sz * fy;
        float uy = sz * fx - sx * fz;
        float uz = sx * fy - sy * fx;
        rm[rmOffset + 0] = sx;
        rm[rmOffset + 1] = ux;
        rm[rmOffset + 2] = -fx;
        rm[rmOffset + 3] = 0.0f;
        rm[rmOffset + 4] = sy;
        rm[rmOffset + 5] = uy;
        rm[rmOffset + 6] = -fy;
        rm[rmOffset + 7] = 0.0f;
        rm[rmOffset + 8] = sz;
        rm[rmOffset + 9] = uz;
        rm[rmOffset + 10] = -fz;
        rm[rmOffset + 11] = 0.0f;
        rm[rmOffset + 12] = 0.0f;
        rm[rmOffset + 13] = 0.0f;
        rm[rmOffset + 14] = 0.0f;
        rm[rmOffset + 15] = 1.0f;
        translateM(rm, rmOffset, -eyeX, -eyeY, -eyeZ);
    }
    static bool invertM(float* mInv, int mInvOffset, float* m,int mOffset) {
        float src0  = m[mOffset +  0];
        float src4  = m[mOffset +  1];
        float src8  = m[mOffset +  2];
        float src12 = m[mOffset +  3];
        float src1  = m[mOffset +  4];
        float src5  = m[mOffset +  5];
        float src9  = m[mOffset +  6];
        float src13 = m[mOffset +  7];
        float src2  = m[mOffset +  8];
        float src6  = m[mOffset +  9];
        float src10 = m[mOffset + 10];
        float src14 = m[mOffset + 11];
        float src3  = m[mOffset + 12];
        float src7  = m[mOffset + 13];
        float src11 = m[mOffset + 14];
        float src15 = m[mOffset + 15];
        float atmp0  = src10 * src15;
        float atmp1  = src11 * src14;
        float atmp2  = src9  * src15;
        float atmp3  = src11 * src13;
        float atmp4  = src9  * src14;
        float atmp5  = src10 * src13;
        float atmp6  = src8  * src15;
        float atmp7  = src11 * src12;
        float atmp8  = src8  * src14;
        float atmp9  = src10 * src12;
        float atmp10 = src8  * src13;
        float atmp11 = src9  * src12;
        float dst0  = (atmp0 * src5 + atmp3 * src6 + atmp4  * src7)
                      - (atmp1 * src5 + atmp2 * src6 + atmp5  * src7);
        float dst1  = (atmp1 * src4 + atmp6 * src6 + atmp9  * src7)
                      - (atmp0 * src4 + atmp7 * src6 + atmp8  * src7);
        float dst2  = (atmp2 * src4 + atmp7 * src5 + atmp10 * src7)
                      - (atmp3 * src4 + atmp6 * src5 + atmp11 * src7);
        float dst3  = (atmp5 * src4 + atmp8 * src5 + atmp11 * src6)
                      - (atmp4 * src4 + atmp9 * src5 + atmp10 * src6);
        float dst4  = (atmp1 * src1 + atmp2 * src2 + atmp5  * src3)
                      - (atmp0 * src1 + atmp3 * src2 + atmp4  * src3);
        float dst5  = (atmp0 * src0 + atmp7 * src2 + atmp8  * src3)
                      - (atmp1 * src0 + atmp6 * src2 + atmp9  * src3);
        float dst6  = (atmp3 * src0 + atmp6 * src1 + atmp11 * src3)
                      - (atmp2 * src0 + atmp7 * src1 + atmp10 * src3);
        float dst7  = (atmp4 * src0 + atmp9 * src1 + atmp10 * src2)
                      - (atmp5 * src0 + atmp8 * src1 + atmp11 * src2);
        float btmp0  = src2 * src7;
        float btmp1  = src3 * src6;
        float btmp2  = src1 * src7;
        float btmp3  = src3 * src5;
        float btmp4  = src1 * src6;
        float btmp5  = src2 * src5;
        float btmp6  = src0 * src7;
        float btmp7  = src3 * src4;
        float btmp8  = src0 * src6;
        float btmp9  = src2 * src4;
        float btmp10 = src0 * src5;
        float btmp11 = src1 * src4;
        float dst8  = (btmp0  * src13 + btmp3  * src14 + btmp4  * src15)
                      - (btmp1  * src13 + btmp2  * src14 + btmp5  * src15);
        float dst9  = (btmp1  * src12 + btmp6  * src14 + btmp9  * src15)
                      - (btmp0  * src12 + btmp7  * src14 + btmp8  * src15);
        float dst10 = (btmp2  * src12 + btmp7  * src13 + btmp10 * src15)
                      - (btmp3  * src12 + btmp6  * src13 + btmp11 * src15);
        float dst11 = (btmp5  * src12 + btmp8  * src13 + btmp11 * src14)
                      - (btmp4  * src12 + btmp9  * src13 + btmp10 * src14);
        float dst12 = (btmp2  * src10 + btmp5  * src11 + btmp1  * src9 )
                      - (btmp4  * src11 + btmp0  * src9  + btmp3  * src10);
        float dst13 = (btmp8  * src11 + btmp0  * src8  + btmp7  * src10)
                      - (btmp6  * src10 + btmp9  * src11 + btmp1  * src8 );
        float dst14 = (btmp6  * src9  + btmp11 * src11 + btmp3  * src8 )
                      - (btmp10 * src11 + btmp2  * src8  + btmp7  * src9 );
        float dst15 = (btmp10 * src10 + btmp4  * src8  + btmp9  * src9 )
                      - (btmp8  * src9  + btmp11 * src10 + btmp5  * src8 );
        float det = src0 * dst0 + src1 * dst1 + src2 * dst2 + src3 * dst3;
        if (det == 0.0f) {
            return false;
        }
        float invdet = 1.0f / det;
        mInv[     mInvOffset] = dst0  * invdet;
        mInv[ 1 + mInvOffset] = dst1  * invdet;
        mInv[ 2 + mInvOffset] = dst2  * invdet;
        mInv[ 3 + mInvOffset] = dst3  * invdet;
        mInv[ 4 + mInvOffset] = dst4  * invdet;
        mInv[ 5 + mInvOffset] = dst5  * invdet;
        mInv[ 6 + mInvOffset] = dst6  * invdet;
        mInv[ 7 + mInvOffset] = dst7  * invdet;
        mInv[ 8 + mInvOffset] = dst8  * invdet;
        mInv[ 9 + mInvOffset] = dst9  * invdet;
        mInv[10 + mInvOffset] = dst10 * invdet;
        mInv[11 + mInvOffset] = dst11 * invdet;
        mInv[12 + mInvOffset] = dst12 * invdet;
        mInv[13 + mInvOffset] = dst13 * invdet;
        mInv[14 + mInvOffset] = dst14 * invdet;
        mInv[15 + mInvOffset] = dst15 * invdet;
        return true;
    }
};
#endif
