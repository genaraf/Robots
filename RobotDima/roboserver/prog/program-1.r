int n, k, p;
n = 50;
outputln "Number 1 of primes:";
input n;
k = 0;
p = 2;
while (k < n)
    if (p == 2)
        output " ", p;
        k = k+1;
        p = p+1;
    else
        int d, prime;
        prime = 1;
        d = 3;
        while (prime == 1 && d*d <= p)
            if (p % d == 0)
                prime = 0;
                break;
            endif
            d = d+2;
        endwhile
        if (prime == 1)
            output " ", p;
            k = k+1;
            if (k % 10 == 0)
                outputln;
            endif
        endif
        p = p+2;
    endif
endwhile
outputln; 

end
