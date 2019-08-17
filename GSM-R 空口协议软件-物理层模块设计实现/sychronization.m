
%x=1:length(I_DATA);
%plot(x,I_DATA);
%axis([1000 1200 -2000 2000])
%grid on;

Iframe=zeros(1,256);
I=zeros(1,length(I_DATA)-255);
Qframe=zeros(1,256);
Q=zeros(1,length(Q_DATA)-255);  
pamax=0;
for j=1:length(I_DATA)-256
    for jj=1:256
        Iframe(jj)=I_DATA(j+jj);
    end
    I=fft(Iframe,256);
    for kk=1:256
        Qframe(kk)=Q_DATA(j+kk);
    end
    Q=fft(Qframe,256);
    a=I.^2+Q.^2;
    ra=real(a);
    ia=imag(a);
    pa=ra.^2+ia.^2;
    for i=1:256
    if(pa(i)>pamax)
        pamax=pa(i);
        fn=j+i;
    end
    end
end