function [I_DATA,Q_DATA]=dataRead()
clear all;
[fid_input,message]=fopen('C:\Users\MAC\Desktop\GSMl1\data\红光镇\932.000MHz_200_20180404_155149.dat','r');
[fid_output, message]=fopen('C:\Users\MAC\Desktop\GSMl1\data\红光镇\932.000MHz_200_20180404_155149.txt','a');
if fid_input==-1
    disp(message);
else
    i=0;%帧编号
    data_len=0;%数据长度
    channel_id=0;%通道号
    comd_codes=0;%命令码
    freq=0;%采集的频率
    freq_dis=0;%频谱跨距
    freq_mid=0;%中频带宽
    sample_rate=0;%采样率

    while ~feof(fid_input)
        rawdata=fread(fid_input,1,'uint32');
        ox_data=dec2hex(rawdata);
        if ox_data=='AAAAAAAA'
           disp('帧头');
           i=i+1;
            rawdata=fread(fid_input,1,'uint16');
            ox_data=dec2hex(rawdata);
            data_len(i)=rawdata;%数据长度 2 字节无符号整数
            
             rawdata=fread(fid_input,1,'uint16');
             ox_data=dec2hex(rawdata);
             channel_id(i)=rawdata;%通道号 2 字节无符号整数
             
              rawdata=fread(fid_input,1,'uint16');
              ox_data=dec2hex(rawdata);
             comd_codes(i)=rawdata;%命令码 2 字节无符号整数
             
              rawdata=fread(fid_input,1,'uint32');
             ox_data=dec2hex(rawdata);
             freq(i)=rawdata;%采集的频率 4 字节无符号整数
             
              rawdata=fread(fid_input,1,'uint32');
              ox_data=dec2hex(rawdata);
             freq_dis(i)=rawdata;%频谱跨距 4 字节无符号整数
             
              rawdata=fread(fid_input,1,'uint32');
             ox_data=dec2hex(rawdata);
             freq_mid(i)=rawdata;%中频带宽 4 字节无符号整数
             
              rawdata=fread(fid_input,1,'uint32');
              ox_data=dec2hex(rawdata);
             sample_rate(i)=rawdata;%采样率 4字节无符号整数
             
             rawdata=fread(fid_input,1,'uint16');
              ox_data=dec2hex(rawdata);
             IQ_len=rawdata;%IQ数据长度 2字节无符号整数
             
             for k=1:IQ_len
                 rawdata=fread(fid_input,1,'int16');
                 %ox_data=dec2hex(rawdata);
                 I_DATA((i-1)*IQ_len+k)=rawdata;
             end
             for k=1:IQ_len
                 rawdata=fread(fid_input,1,'int16');
                 %ox_data=dec2hex(rawdata);
                 Q_DATA((i-1)*IQ_len+k)=rawdata;
             end    
             
        elseif ox_data=='55555555'
            disp('帧尾');
        end
        if i==204 break;end;
        
    end
end
fclose(fid_input);
fclose(fid_output);