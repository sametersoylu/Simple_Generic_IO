#ifndef SUM_H
int sumi(int a, int b);
long double sumld(long double a, long double b);
double sumd(double a, double b);
float sumf(float a, float b);


#define sum(x, y) _Generic((x), \
                        int : sumi, \
                        long double : sumld, \
                        double : sumd, \
                        float : sumf \
                        )(x, y)
#endif