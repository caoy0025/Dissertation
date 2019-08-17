%���Ƶ��Ʋ���00100101
clear all;
close all;
Ak = textread('C:\Users\MAC\Desktop\TEst\test.txt');               %���������ź�
Ts=1/16000;                         %�����ź�����Ϊ1/16000s,��Ϊ16KHz
Tb=1/32000;                         %�����ź�����ΪTs/2=1/32000s,��32KHz
BbTb=0.3;                           %ȡBbTbΪ0.3
Bb=BbTb/Tb;                         %3dB����
Fc=32000;                           %�ز�Ƶ��Ϊ32KHz
F_sample=64;                        %ÿ�ز�����64����
B_num=length(Ak);                            %�����ź�Ϊ8����Ԫ
B_sample=F_sample*Fc*Tb;            %ÿ������Ԫ��������B_sample=Tb/Dt
Dt=1/Fc/F_sample;                   %�������
t=0:Dt:B_num*Tb-Dt;                 %����ʱ��
T=Dt*length(t); %����ʱ��ֵ
Ak=2*Ak-1;                   %˫������Ԫ
gt=ones(1,B_sample);         %ÿ��Ԫ��Ӧ���ز��ź�
%Akk=sigexpand(Ak,B_sample);   %��Ԫ��չ
%temp=conv(Akk,gt);                 %��Ԫ��չ
%Akk=temp(1:length(Akk));            %��Ԫ��չ
Akk=rectpulse(Ak,B_sample);

tt=-2.5*Tb:Dt:2.5*Tb-Dt;   
%g(t)=Q[2*pi*Bb*(t-Tb/2)/sqrt(log(2))]-Q[2*pi*Bb*(t+Tb/2)/sqrt(log(2))];
%Q(t)=erfc(t/sqrt(2))/2;
%gausst=erfc(2*pi*Bb*(tt-Tb/2)/sqrt(log(2))/sqrt(2))/2-erfc(2*pi*Bb*(tt+Tb/2)/sqrt(log(2))/sqrt(2))/2;         
a=sqrt(log(2)/2)/Bb;
gausst=sqrt(pi)/a*exp(-(pi/a*tt).^2);
gausst=gausst/1.1/max(gausst);

J_g=zeros(1,length(gausst)); %ʹJ_g �ĳ��Ⱥ�Gausst��һ��
for i=1:length(gausst)
    if i==1 
        J_g(i)=gausst(i)*Dt;
    else
        J_g(i)=J_g(i-1)+gausst(i)*Dt;%��弤��Ӧ
    end;
end;
J_g=J_g/2/Tb;

%������λAlpha
Alpha=zeros(1,length(Akk));
k=1;
L=0;
for j=1:B_sample
    J_Alpha=Ak(k+2)*J_g(j);    %��λ��Ӧ
    Alpha((k-1)*B_sample+j)=pi*J_Alpha+rem(L,4)*pi/2;
end; 
%
k=2;
L=0;
for j=1:B_sample
    J_Alpha=Ak(k+2)*J_g(j)+Ak(k+1)*J_g(j+1*B_sample);
    Alpha((k-1)*B_sample+j)=pi*J_Alpha+rem(L,4)*pi/2;
end;  

k=3;
L=0;
for j=1:B_sample
    J_Alpha=Ak(k+2)*J_g(j)+Ak(k+1)*J_g(j+1*B_sample)+Ak(k)*J_g(j+2*B_sample);
    Alpha((k-1)*B_sample+j)=pi*J_Alpha+rem(L,4)*pi/2;
end;  

k=4;
L=0;
for j=1:B_sample
    J_Alpha=Ak(k+2)*J_g(j)+Ak(k+1)*J_g(j+B_sample)+Ak(k)*J_g(j+2*B_sample)+Ak(k-1)*J_g(j+3*B_sample);
    Alpha((k-1)*B_sample+j)=pi*J_Alpha+rem(L,4)*pi/2;
end;

L=0;%�ۻ���Ԫ��(֮ǰ������Ԫ֮�ͣ����㸽����λ)
for k=5:B_num-2
    if k==5
        L=0;
    else
        L=L+Ak(k-3);
    end;
    for j=1:B_sample
        J_Alpha=Ak(k+2)*J_g(j)+Ak(k+1)*J_g(j+1*B_sample)+Ak(k)*J_g(j+2*B_sample)+Ak(k-1)*J_g(j+3*B_sample)+Ak(k-2)*J_g(j+4*B_sample);
        Alpha((k-1)*B_sample+j)=pi*J_Alpha+rem(L,4)*pi/2;
    end;    
end;

%B_num-1;
k=B_num-1;
L=L+Ak(k-3);
for j=1:B_sample
    J_Alpha=Ak(k+1)*J_g(j+B_sample)+Ak(k)*J_g(j+2*B_sample)+Ak(k-1)*J_g(j+3*B_sample)+Ak(k-2)*J_g(j+4*B_sample);
   Alpha((k-1)*B_sample+j)=pi*J_Alpha+rem(L,4)*pi/2;
end;  

%B_num;
k=B_num;
L=L+Ak(k-3);
for j=1:B_sample
    J_Alpha=Ak(k)*J_g(j+2*B_sample)+Ak(k-1)*J_g(j+3*B_sample)+Ak(k-2)*J_g(j+4*B_sample);
    Alpha((k-1)*B_sample+j)=pi*J_Alpha+rem(L,4)*pi/2;
end;  

%
S_Gmsk=cos(2*pi*Fc*t+Alpha);
subplot(311)
plot(t/Tb,Akk);
axis([0 B_num -1.5 1.5]);
title('��������');

subplot(312)
Alpha=rem(Alpha,2*pi);
plot(t/Tb,Alpha*2/pi);
axis([0 B_num min(Alpha*2/pi)-1 max(Alpha*2/pi)+1]);
title('��λ����');

subplot(313)
plot(t/Tb,S_Gmsk);
axis([0 B_num -1.5 1.5]);
title('GMSK����');

%���
for n=1:length(Alpha);
    if n<=B_sample
        Alpha1(n)=0;
    else Alpha1(n)=Alpha(n-B_sample);
    end
end

a=zeros(1,B_num);
for l=2:B_num
    a(l)=1;
end

%ak=sigexpand(a,B_sample);   %��Ԫ��չ
%temp=conv(ak,gt);                 %��Ԫ��չ
%ak=temp(1:length(ak));
ak=rectpulse(a,B_sample);
S_Gmsk1=cos(2*pi*Fc*(t-Tb)+Alpha1+pi/2).*ak; %�ӳ�1bt,����pi/2
figure
subplot(311)
plot(t/Tb,S_Gmsk1);
axis([0 B_num -1.5 1.5]);
title('�ӳ�1bt,����pi/2GMSK����');

xt=S_Gmsk1.*S_Gmsk;
x=0;
subplot(312)
plot(t/Tb,xt,t/Tb,x,'r:');
axis([0 B_num -1.5 1.5]);
title('��˺���');

%��ͨ�˲�
Fs=10000;
rp=3;rs=50;
wp=2*pi*50;ws=2*pi*800;
[n,wn]=buttord(wp,ws,rp,rs,'s')
[z,p,k]=buttap(n);  
[bp,ap]=zp2tf(z,p,k);  
[bs,as]=lp2lp(bp,ap,wn);  
[b,a]=bilinear(bs,as,Fs)   
y=filter(b,a,xt);
subplot(313)
plot(t/Tb,y,t/Tb,x,'r:');
axis([0 B_num -1.5 1.5]);
title('������ͨ�˲�������');

for i=1:B_num
    if y(i*B_sample)>0
        bt(i)=1
    else
        bt(i)=0
    end
end
bt=2*bt-1;
%btt=sigexpand(bt,B_sample);   %��Ԫ��չ
%temp1=conv(btt,gt);           %��Ԫ��չ
%btt=temp1(1:length(btt));     %��Ԫ��չ
btt=rectpulse(bt,B_sample);
figure
subplot(311)
plot(bt)
title('����ֵ');
axis([0 B_num -1.5 1.5]);
subplot(312)
plot(t/Tb,Akk);
axis([0 B_num -1.5 1.5]);
title('ԭ��������');
subplot(313)
plot(t/Tb,btt);
axis([0 B_num -1.5 1.5]);
title('�������');


