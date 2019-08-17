%绘制调制波形00100101
clear all;
close all;
Ak = textread('C:\Users\MAC\Desktop\TEst\test.txt');               %产生基带信号
Ts=1/16000;                         %基带信号周期为1/16000s,即为16KHz
Tb=1/32000;                         %输入信号周期为Ts/2=1/32000s,即32KHz
BbTb=0.3;                           %取BbTb为0.3
Bb=BbTb/Tb;                         %3dB带宽
Fc=32000;                           %载波频率为32KHz
F_sample=64;                        %每载波采样64个点
B_num=length(Ak);                            %基带信号为8个码元
B_sample=F_sample*Fc*Tb;            %每基带码元采样点数B_sample=Tb/Dt
Dt=1/Fc/F_sample;                   %采样间隔
t=0:Dt:B_num*Tb-Dt;                 %仿真时间
T=Dt*length(t); %仿真时间值
Ak=2*Ak-1;                   %双极性码元
gt=ones(1,B_sample);         %每码元对应的载波信号
%Akk=sigexpand(Ak,B_sample);   %码元扩展
%temp=conv(Akk,gt);                 %码元扩展
%Akk=temp(1:length(Akk));            %码元扩展
Akk=rectpulse(Ak,B_sample);

tt=-2.5*Tb:Dt:2.5*Tb-Dt;   
%g(t)=Q[2*pi*Bb*(t-Tb/2)/sqrt(log(2))]-Q[2*pi*Bb*(t+Tb/2)/sqrt(log(2))];
%Q(t)=erfc(t/sqrt(2))/2;
%gausst=erfc(2*pi*Bb*(tt-Tb/2)/sqrt(log(2))/sqrt(2))/2-erfc(2*pi*Bb*(tt+Tb/2)/sqrt(log(2))/sqrt(2))/2;         
a=sqrt(log(2)/2)/Bb;
gausst=sqrt(pi)/a*exp(-(pi/a*tt).^2);
gausst=gausst/1.1/max(gausst);

J_g=zeros(1,length(gausst)); %使J_g 的长度和Gausst的一样
for i=1:length(gausst)
    if i==1 
        J_g(i)=gausst(i)*Dt;
    else
        J_g(i)=J_g(i-1)+gausst(i)*Dt;%求冲激响应
    end;
end;
J_g=J_g/2/Tb;

%计算相位Alpha
Alpha=zeros(1,length(Akk));
k=1;
L=0;
for j=1:B_sample
    J_Alpha=Ak(k+2)*J_g(j);    %相位响应
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

L=0;%累积码元和(之前所有码元之和，计算附加相位)
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
title('基带波形');

subplot(312)
Alpha=rem(Alpha,2*pi);
plot(t/Tb,Alpha*2/pi);
axis([0 B_num min(Alpha*2/pi)-1 max(Alpha*2/pi)+1]);
title('相位波形');

subplot(313)
plot(t/Tb,S_Gmsk);
axis([0 B_num -1.5 1.5]);
title('GMSK波形');

%解调
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

%ak=sigexpand(a,B_sample);   %码元扩展
%temp=conv(ak,gt);                 %码元扩展
%ak=temp(1:length(ak));
ak=rectpulse(a,B_sample);
S_Gmsk1=cos(2*pi*Fc*(t-Tb)+Alpha1+pi/2).*ak; %延迟1bt,移相pi/2
figure
subplot(311)
plot(t/Tb,S_Gmsk1);
axis([0 B_num -1.5 1.5]);
title('延迟1bt,移相pi/2GMSK波形');

xt=S_Gmsk1.*S_Gmsk;
x=0;
subplot(312)
plot(t/Tb,xt,t/Tb,x,'r:');
axis([0 B_num -1.5 1.5]);
title('相乘后波形');

%低通滤波
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
title('经过低通滤波器后波形');

for i=1:B_num
    if y(i*B_sample)>0
        bt(i)=1
    else
        bt(i)=0
    end
end
bt=2*bt-1;
%btt=sigexpand(bt,B_sample);   %码元扩展
%temp1=conv(btt,gt);           %码元扩展
%btt=temp1(1:length(btt));     %码元扩展
btt=rectpulse(bt,B_sample);
figure
subplot(311)
plot(bt)
title('抽样值');
axis([0 B_num -1.5 1.5]);
subplot(312)
plot(t/Tb,Akk);
axis([0 B_num -1.5 1.5]);
title('原基带波形');
subplot(313)
plot(t/Tb,btt);
axis([0 B_num -1.5 1.5]);
title('解调后波形');


