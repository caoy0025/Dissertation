N=100000;
Iframe=zeros(1,256);
I=zeros(1,length(I_DATA)-255);
aa=zeros(1,N);
pmax=zeros(1,N);
Qframe=zeros(1,256);
Q=zeros(1,length(Q_DATA)-255);  
pamax=0;
for k=1:N
    for jj=1:256
        Iframe(jj)=I_DATA(k+jj);
    end
    %I=fft(Iframe,256);
    for kk=1:256
        Qframe(kk)=Q_DATA(k+kk);
    end
    %Q=fft(Qframe,256);
    A=Iframe+j*Qframe;
    a=fft(A,256);
    b=real(a).^2+imag(a).^2;
    aa(k)=sum(b)/256;
    pmax(k)=max(b);
    
     x=1:256;
 %   plot(x,b-aa(k));
   end

pamax=max(pmax);
[x,y]=find(pamax==pmax);
plot(1:N,pmax);
