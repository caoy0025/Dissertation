I=zeros(1,2*length(I_DATA));
Q=zeros(1,2*length(Q_DATA));
for a=1:length(I_DATA)
    I(2*a-1)=I_DATA(a);
    I(2*a)=I_DATA(a);
    Q(2*a-1)=Q_DATA(a);
    Q(2*a)=Q_DATA(a);
end
d=3;

% F_sample=16;
% Fc=932000000;
% Dt=1/Fc/F_sample; 
% Tb=1/Fc;
% B_num=length(I)/F_sample; 
% t=1:1:length(I);
% B_sample=F_sample*Fc*Tb;
% t=0:Dt:B_num*Tb-Dt; 
% x=0;
% 
% Fs=1;
% rp=3;rs=50;
% wp=2*pi*50;ws=2*pi*800;
% [n,wn]=buttord(wp,ws,rp,rs,'s')
% [z,p,k]=buttap(n);  
% [bp,ap]=zp2tf(z,p,k);  
% [bs,as]=lp2lp(bp,ap,wn);  
% [b,a]=bilinear(bs,as,Fs)
% 
% I=filter(b,a,I);
% 
% % subplot(211)
% % plot(I);
% % axis([0 B_num -1.5 1.5]);
% % title('I֧·������ͨ�˲�������');
% 
% Q=filter(b,a,Q);
% % subplot(212)
% % plot(t/Tb,y2,t/Tb,x,'r:');
% % axis([0 B_num -1.5 1.5]);
% % title('Q֧·������ͨ�˲�������');
% % Alpha=zeros(1,length(I));
% % a=zeros(1,B_num);
% % for l=2:B_num
% %     a(l)=1;
% % end
% % ak=rectpulse(a,B_sample);
% % %S_Gmsk1=cos(2*pi*Fc*(t-Tb)+Alpha1+pi/2).*ak; %�ӳ�1bt,����pi/2
I_D=zeros(1,length(I));
for ii=1:length(I)-d
    I_D(ii+d)=I(ii);
end

Q_D=zeros(1,length(I));
for qq=1:length(I)-d
    Q_D(qq+d)=Q(qq);
end
I1=I_D;
Q1=Q_D;
% 
% figure
% subplot(411)
% plot(t/Tb,I1);
% axis([0 B_num -1.5 1.5]);
% title('I�ӳ�1bt,����pi/2GMSK����')
% 
% subplot(412)
% plot(t/Tb,Q1);
% axis([0 B_num -1.5 1.5]);
% title('Q�ӳ�1bt,����pi/2GMSK����')
% 
% x1t=I1.*Q;
% x=0;
% subplot(413)
% plot(t/Tb,x1t,t/Tb,x,'r:');
% axis([0 B_num -1.5 1.5]);
% title('I_D��Q��˺���');
% 
% x2t=Q1.*I;
% x=0;
% subplot(414)
% plot(t/Tb,x2t,t/Tb,x,'r:');
% axis([0 B_num -1.5 1.5]);
% title('Q_D��I��˺���');
% 
% Fs=10000;
% rp=3;rs=50;
% wp=2*pi*50;ws=2*pi*800;
% [n,wn]=buttord(wp,ws,rp,rs,'s')
% [z,p,k]=buttap(n);  
% [bp,ap]=zp2tf(z,p,k);  
% [bs,as]=lp2lp(bp,ap,wn);  
% [b,a]=bilinear(bs,as,Fs)
% 
% y1=filter(b,a,x1t);
% subplot(411)
% plot(t/Tb,y1,t/Tb,x,'r:');
% axis([0 B_num -1.5 1.5]);
% title('I֧·������ͨ�˲�������');
% 
% y2=filter(b,a,x2t);
% subplot(412)
% plot(t/Tb,y2,t/Tb,x,'r:');
% axis([0 B_num -1.5 1.5]);
% title('Q֧·������ͨ�˲�������');
% 
% d=y1+y2;

num=0;
z1=angle(I+j*Q)/(2*pi);
z2=angle(I1+j*Q1)/(2*pi);
z=z1-z2;

% num=0;
% for i=1:length(I)
%     if z(i)==-0.5;
%         num=num+1;
%     end
% end
% 
% for i=1:length(I)
%     if z(i)==0.5
%         bt(i)=1
%     elseif z(i)==-0.5
%         bt(i)=-1
%     else
%         bt(i)=0    
%     end
% end 

% iii=0;
% for ii=1:length(I)
%     if bt(ii)==1
%         tt(iii)=1;
%         iii=iii+1;
%     elseif bt(ii)==-1
%         tt(iii)=-1
%         iii=iii+1;
%     end
% end 
for x=1:length(I)/3
    temp(x)=(z(3*x)+z(3*x-1)+z(3*x-2)/3);
end
plot(temp);


for i=1:length(I)/3

    if temp(i)>0
        bt(i)=1
    else
        bt(i)=0
    end
end    

figure
subplot(411)
plot(z1)
title('����IQ֧·�������λͼ');
subplot(412)
plot(z2)
title('�ӳ�1bit�����λͼ');
subplot(413)
plot(temp)
title('��λ��ͼ');
subplot(414)
plot(bt)
title('�������ͼ');
 
% for i=1:length(I)
%     if d(i)>0
%         bt(i)=1
%     else
%         bt(i)=0
%     end
% end
% 



% btt=rectpulse(bt,B_sample);
% subplot(413)
% plot(bt)
% title('����ֵ');
% subplot(414)
% plot(t/Tb,btt);
% axis([0 B_num -1.5 1.5]);
% title('�������');