function [I_DATA,Q_DATA]=dataRead()
clear all;
[fid_input,message]=fopen('C:\Users\MAC\Desktop\GSMl1\data\�����\932.000MHz_200_20180404_155149.dat','r');
[fid_output, message]=fopen('C:\Users\MAC\Desktop\GSMl1\data\�����\932.000MHz_200_20180404_155149.txt','a');
if fid_input==-1
    disp(message);
else
    i=0;%֡���
    data_len=0;%���ݳ���
    channel_id=0;%ͨ����
    comd_codes=0;%������
    freq=0;%�ɼ���Ƶ��
    freq_dis=0;%Ƶ�׿��
    freq_mid=0;%��Ƶ����
    sample_rate=0;%������

    while ~feof(fid_input)
        rawdata=fread(fid_input,1,'uint32');
        ox_data=dec2hex(rawdata);
        if ox_data=='AAAAAAAA'
           disp('֡ͷ');
           i=i+1;
            rawdata=fread(fid_input,1,'uint16');
            ox_data=dec2hex(rawdata);
            data_len(i)=rawdata;%���ݳ��� 2 �ֽ��޷�������
            
             rawdata=fread(fid_input,1,'uint16');
             ox_data=dec2hex(rawdata);
             channel_id(i)=rawdata;%ͨ���� 2 �ֽ��޷�������
             
              rawdata=fread(fid_input,1,'uint16');
              ox_data=dec2hex(rawdata);
             comd_codes(i)=rawdata;%������ 2 �ֽ��޷�������
             
              rawdata=fread(fid_input,1,'uint32');
             ox_data=dec2hex(rawdata);
             freq(i)=rawdata;%�ɼ���Ƶ�� 4 �ֽ��޷�������
             
              rawdata=fread(fid_input,1,'uint32');
              ox_data=dec2hex(rawdata);
             freq_dis(i)=rawdata;%Ƶ�׿�� 4 �ֽ��޷�������
             
              rawdata=fread(fid_input,1,'uint32');
             ox_data=dec2hex(rawdata);
             freq_mid(i)=rawdata;%��Ƶ���� 4 �ֽ��޷�������
             
              rawdata=fread(fid_input,1,'uint32');
              ox_data=dec2hex(rawdata);
             sample_rate(i)=rawdata;%������ 4�ֽ��޷�������
             
             rawdata=fread(fid_input,1,'uint16');
              ox_data=dec2hex(rawdata);
             IQ_len=rawdata;%IQ���ݳ��� 2�ֽ��޷�������
             
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
            disp('֡β');
        end
        if i==204 break;end;
        
    end
end
fclose(fid_input);
fclose(fid_output);