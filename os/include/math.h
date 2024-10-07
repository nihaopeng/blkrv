#ifndef _MATH_H_
#define _MATH_H_

int abs(int a){
    return a>0?a:-a;
}

int mod(int x,int y){
    while(x>=y)
    {
        x-=y;
    }
    return x;
}

int div(int src,int dst){
    int quo=0;
    int sign = ((src ^ dst) < 0);			//判断符号
	int res = 0;							//结果
	int dived = abs(src);					//被除数
	int div = abs(dst);						//除数

	for (int i = 10; i >= 0; i--) {			//类似贪心算法，移位求解
		if (dived >> i >= dst) {
			res = res + (1 << i);
			dived = dived - (div << i);
		}
	}
	sign ? (quo = -res) : (quo = res);
    return quo;
}

int mul(int src, int dst) {
    int pro=0;
	int sign = ((src ^ dst) < 0);			//判断符号
	int res = 0;							//结果
	int muled = abs(src);					//被乘数
	int mul = abs(dst);						//乘数

	for (int i = 10; i >= 0; i--) {			//类似贪心算法，移位求解
		if ((1 << i) <= dst) {
			res = res + muled << i;
			mul = mul - (1 << i);
		}
	}
	sign ? (pro = -res) : (pro = res);
    return pro;
}

#endif // !_MATH_H_
