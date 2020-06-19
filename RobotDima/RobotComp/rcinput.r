int a, t;

while(1==1)
    outputln "1. RC Test";
    outputln "2. Sound Test";
    outputln "3. Mouth Test";
    outputln;
    outputln "0. Exit";
    output "plese enter test number:";
    input t;
    if(t <= 0) 
        break;
    endif
    if(t == 1) 
        outputln "RC Test command range (0-20)";
    elseif (t == 2)
        outputln "Sound Test command range (0-18)";
    elseif (t == 3)
        outputln "Mouth Test command range (0-30)";
    endif
    outputln "For exit enter: -1";
    
    while(1==1)
        output "plese enter command:";
        input a;
        if(a < 0) 
            break;
        endif
        if(t == 1) 
            RC(a, 2000);
        elseif (t == 2)
            sound(a);
        elseif (t == 3)
            mouth(a);
        endif
    endwhile
endwhile
RC(0, 2000);
end
