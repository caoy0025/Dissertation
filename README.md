# Dissertation

This profile includes Mr. CAO YUN's Bachelor and Master dissertation:

I)	GSM-R Air Interface Protocol Software - Physical Layer Module Design and Implementation.

This dissertation is finished in Southwest Jiaotong University(SWJTU) in Chengdu, China.

According to 3GPP protocols, this paper proposes the analysis software for GSM-R Um air interface in Physical Layer (L1) based on Visual Studio 2017 and MATLAB 2014, which has the function of sending and receiving messages. This paper realizes: 

1)	channel coding and decoding, 

2)	interleaving and de-interleaving, 

3)	channel mapping, 

4)	modulation and demodulation,

5)	synchronization of received signals 
and implements data test based on these functions.

The Physical Layer module discussed in this paper is mainly divided into two parts: sending end and receiving end. 

The transmitter is programmed mainly in the C language, and the channel coding uses (2,1,4) convolution coding method. Inner interleaving and inter- block interleaving are designed respectively. The channel mapping is completed during inter- block interleaving. On the MATLAB platform, GMSK modulation is realized through the phase table, and 1- bit difference method is used for demodulation. At the receiving end, the full zero data characteristic of FCCH is used for synchronization step 1, and the timing accurate synchronization can be finished by using the autocorrelation of the SCH training sequence. De-interleaving can be regarded as an inverse process of interleaving, and the Viterbi algorithm is used for channel decoding. Finally, 23 bytes of data format are delivered to the Data Link Layer (L2) for further analysis. The test data of Xipu town are analyzed by the software, the results satisfy the theoretical expectation.

II)	Design of Information Hiding in Image

This dissertation is finished in Nanyang Technological University(NTU), Singapore.

Based on the previous work, this paper completed the following work in response to the current development of digital watermarking.

1) Understand the characteristics, classification and application of digital watermarking technology, review the research background of digital watermarking technology, study several common transformations of image domain watermarking in transform domain and image scrambling technology, based on existing algorithms. An image digital watermarking algorithm combining discrete wavelet transform, discrete cosine transform and matrix singular value decomposition and new scrambling technique is proposed. According to the shortcomings of related algorithms, grayscale scrambling and pseudo magic square transform is used to improve the robustness of the algorithm.

2) Experiments based on Python 3.7 are made to test the proposed watermarking algorithm. All the experimental results show that the image watermarking algorithm based on DCT-DWT-SVD and scrambling proposed in this paper has strong imperceptibility and robustness. When the watermark is added, the image undergoes common signal processing such as chopping, rotation, filtering, etc. After that, the presence of the watermark can still be detected and the calculation speed is faster.
