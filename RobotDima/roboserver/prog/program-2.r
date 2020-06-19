double x0, x, a;
double EPS;
EPS = 0.0000001;
while (0 == 0)
    outputln "Input a number:";
    input a;
    if (a <= 0.)
        break;
    endif
    x = a;
    if (x < 1.)
        x = 1.;
    endif
    double xPrev;
    xPrev = x + 1.;
    while (xPrev - x > EPS)
        xPrev = x;  
        x = (x + a/x) / 2.;
        outputln x;
    endwhile
    outputln "sqrt(", a, ") = ", x;
endwhile
end
