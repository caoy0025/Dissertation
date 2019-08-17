
%x=1:length(I_DATA);
%plot(x,I_DATA);
%axis([1000 1200 -2000 2000])
%grid on;

N=10000000;
Iframe=zeros(1,256);
I=zeros(1,length(I_DATA)-255);
aa=zeros(1,N);
pmax=zeros(1,N);
Qframe=zeros(1,256);
Q=zeros(1,length(Q_DATA)-255);  
test=zeros(1,N);
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
    %plot(x,b-aa(k));
    %a=I.^2+Q.^2;
    %ra=real(a);
    %ia=imag(a);
    %pa=ra.^2+ia.^2;
    %test(j)=pa(1);
   % for i=1:256
    %    if(pa(i)>pamax)
   %         pamax=pa(i);

  %      end
  %  end
end

pamax=max(pmax);
[x,y]=find(pamax==pmax);
plot(1:N,pmax);
%x=1:25000;
%plot(x,aa);
%axis([0 10000 0 8000])
%x=1:25000;
%plot(x,test);
%axis([1000 1200 -2000 2000])