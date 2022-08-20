### PS的格式解析总结

PS流格式定义在MPEG2中，详见 iso 13818-1。

针对H264 做如下PS 封装：

视频 的情况

每个IDR NALU 前一般都会包含SPS、PPS 等NALU，因此将SPS、PPS、IDR 的NALU 封装为一个PS 包，包括ps 头，然后加上PS system header，PS system map，PES header，h264 raw data。

所以一个IDR NALU PS 包由外到内顺序是：PS header| PS system header | PS system Map | PES header | h264 raw data。

对于其它非关键帧的PS 包，直接加上PS头和PES 头就可以了。顺序为：PS header | PES header | h264raw data。

当有音频数据时，将数据加上PES header 放到视频 PES 后。

顺序如下：PS 包=PS头|PES(video)|PES(audio)，再用 RTP 封装发送就可以了。

PS 格式的各个部分：

#### 1、PS header：

节目流包（PS header）中各字段的语义定义:

![ps_header](.\pic_resource\PS_note\ps_header.png)

**pack_start_code** : 包起始码字段，值为 0x000001BA，用来标志一个包的开始。

**system_clock_reference_base**：系统时钟参考字段 ，system_clock_reference_extenstion：系统时钟参考(SCR)是一个分两部分编码的42位字段。第一部分system_clock_reference_base是一个长度为33位的字段，其值SCR_base(i)；第二部分system_clock_reference_extenstion是一个长度为9位的字段，其值SCR_ext(i)。SCR字段指出了基本流中包含ESCR_base最后一位的字节到达节目目标解码器输入端的期望时间。

**marker_bit**：标记位字段，1位字段，取值'1'。

**program_mux_rate**：节目复合速率字段，一个22位整数，规定P-STD在包含该字段的包期间接收节目流的速率。其值以50字节/秒为单位。不允许取0值。该字段所表示的值用于在2.5.2中定义P-STD输入端的字节到达时间。该字段值在本标准中的节目多路复合流的不同包中取值可能不同。

**pack_stuffing_length**：包填充长度字段，3位整数，规定该字段后填充字节的个数。

**stuffing_byte**：填充字节字段，8位字段，取值恒为'1111 1111'。该字段能由编码器插入，例如为了满足通道的要求。它由解码器丢弃。在每个包标题中最多只允许有7个填充字节。

#### 2、PS system header

PS系统头：节目流系统标题部分的定义：

![PS system header](.\pic_resource\PS_note\ps_system_header.png)

系统标题中各字段的语义定义:

**system_header_start_code**：系统标题起始码字段，取值0x000001BB，指出系统标题的开始。

**header_length**：标题长度字段，16位字段。指出该字段后的系统标题的字节长度。在本规范将来的扩充中可能扩展该字段。

**rate_bound**：速率界限字段，22位字段，取值不小于编码在节目流的任何包中的 program_mux_rate 字段的最大值。该字段可被解码器用于估计是否有能力对整个流解码。

**audio_bound**：音频界限字段，6位字段，取值是在从0到32的闭区间中的整数，且不小于节目流中解码过程同时活动的GB/T XXXX.3和GB/T AAAA.3音频流的最大数目。在本小节中，若STD缓冲区非空或展现单元正在P-STD模型中展现，则GB/T XXXX.3和GB/T AAAA.3音频流的解码过程是活动的。

**fixed_flag**：固定标志字段，1位标志位。置'1'时表示比特率恒定的操作；置'0'时，表示操作的比特率可变。在恒定比特率的操作期间，复合的GB/T XXXX.1流中的system_clock_reference字段值应遵从下面的线性公式:

*SCR_base*(*i*)＝((*c1*×*i*＋*c2*) DIV 300) % 233     (2-22)

*SCR_ext*(*i*)＝((*c1*×*i*＋*c2*) DIV 300) % 300        (2-23)

其中：

c1  对所有i均有效的实型常数；

c2  对所有i均有效的实型常数；

**CSPS_flag** ：CSPS标志字段，1位字段。置'1'时，节目流符合2.7.9中定义的限制。

**system_audio_lock_flag**：系统音频锁定标志字段，1位字段。表示在系统目标解码器的音频采样率和system_clock_frequency之间存在规定的比率。system_clock_frequency在2.5.2.1中定义而音频采样率由GB/T XXXX.3规定。如果对节目流中所有音频基本流的所有展现单元，system_clock_frequency和实际音频采样率的比例SCASR是恒定的，且对音频流中所指出的标准采样率和下表中数值相等，则该字段只能为'1'。

*SCASR*＝(*system_clock_frequency*) / *audio_sample_rate_in_the_P-STD*  (2-24)

记号X/Y表示实数除法。

| 标准音频采样频率(kHz) | 16                       | 32                      | 22.05                  | 44.1                   | 24                     | 48                       |
| --------------------- | ------------------------ | ----------------------- | ---------------------- | ---------------------- | ---------------------- | ------------------------ |
| SCASR                 | 27 000 000--------16 000 | 27 000 000-------32 000 | 27 000 000------22 050 | 27 000 000------44 100 | 27 000 000------24 000 | 27 000 000--------48 000 |

 

**system_video_lock_flag**：系统视频锁定标志字段，1位字段。表示在系统目标解码器的视频帧速率和system_clock_frequency之间存在规定的比率。system_clock_frequency在2.5.2.1中定义而视频帧速率由GB/T XXXX.2规定。如果对GB/T XXXX.1中所有视频基本流的所有展现单元，system_clock_frequency和实际视频帧速率的比例SCFR是恒定的，且对视频流中所指出的标准帧速率和下表中数值相等，则该字段只能为'1'。

*SCFR*＝*system_clock_frequency* / *frame_rate_in_the_P-STD*       (2-25)

| 标准帧速率(Hz) | 23.976    | 24        | 25        | 29.97   | 30      | 50      | 59.94   | 60      |
| -------------- | --------- | --------- | --------- | ------- | ------- | ------- | ------- | ------- |
| SCFR           | 1 126 125 | 1 125 000 | 1 080 000 | 900 900 | 900 000 | 540 000 | 450 450 | 450 000 |

比率SCFR的值是精确的。对于23.976，29.97或59.94帧/秒的标准速率，实际的帧速率与标准速率略有不同。

**video_bound**：视频界限字段

5位字段，取值是在从0到16的闭区间中的整数且不小于节目流中解码过程同时活动的GB/T XXXX.2和GB/T AAAA.2流的最大数目。在本小节中，若P-STD缓冲区非空或展现单元正在P-STD模型中展现，则GB/T XXXX.2和GB/T AAAA.2视频流的解码过程是活动的。

**packet_rate_restriction_flag**：分组速率限制标志字段，1位标志位。若CSPS标识为'1'，则该字段表示2.7.9中规定的哪个限制适用于分组速率。若CSPS标识为'0'，则该字段的含义未定义。

**reserved_bits**：保留位字段，7位字段。被保留供ISO/IEC将来使用。它的值应为'111 1111'，除非ISO/IEC对它作出其它规定。

**stream_id**：流标识字段，8位字段。指示其后的P-STD_buffer_bound_scale和P-STD_buffer_size_bound字段所涉及的流的编码和基本流号码。

若取值'1011 1000'，则其后的P-STD_buffer_bound_scale和P-STD_buffer_size_bound字段指节目流中所有的音频流。

若取值'1011 1001'，则其后的P-STD_buffer_bound_scale和P-STD_buffer_size_bound字段指节目流中所有的视频流。

若stream_id取其它值，则应该是大于或等于'1011 1100'的一字节值且应根据表2-18解释为流的编码和基本流号码。

节目流中的每个基本流应在每个系统标题中通过这种机制精确地规定一次它的P-STD_buffer_bound_scale和P-STD_buffer_size_bound。

**P-STD_buffer_bound_scale**：P-STD缓冲区界限比例字段，1位字段。表示用于解释后续P-STD_buffer_size_bound字段的比例系数。若前面的stream_id表示一个音频流，则该字段值为'0'。若表示一个视频流，则该字段值为'1'。对于所有其它的流类型，该字段值可以为'0'也可以为'1'。

**P-STD_buffer_size_bound**：P-STD缓冲区大小界限字段，13位无符号整数，取值不小于节目流中流n的所有分组的P-STD缓冲区大小BSn的最大值。若P-STD_buffer_bound_scale的值为'0'，则该字段以128字节为单位来度量缓冲区大小的边界。若P-STD_buffer_bound_scale的值为'1'，则该字段以1024字节为单位来度量缓冲区大小的边界。因此：

```
if (P-STD_buffer_bound_scale = = 0)
	BSn ≤ P-STD_buffer_size_bound × 128
else
	BSn ≤ P-STD_buffer_size_bound × 1024
```

所以对于系统头部的解析，我们一般只要先首先判断是否存在系统头（根据系统头的起始码0x000001BB），然后我们读取系统头的头部长度，即PS SYSTEM HEADER LENGTH部分，然后根据头部的长度，跳过PS系统头。进入下一个部分，即PS 节目流映射头。

#### 3、PS program stream map

节目流映射 

![program stream map](\pic_resource\PS_note\ps_program_stream_map.png) 

节目流映射中各字段的语义定义：

**packet_start_code_prefix**：分组起始码前缀字段，24位码。它和跟随其后的map_stream_id共同组成一个分组起始码以标志分组的开始。该字段是值为'0000 0000 0000 0000 0000 0001' (0x000001)的位串。

**map_stream_id**：映射流标识字段，8位字段，值为0xBC。

**program_stream_map_length**：节目流映射长度字段，16位字段。指示紧跟在该字段后的program_stream_map中的字节数。该字段的最大值为1018(0x3FA)。

**current_next_indicator**：当前下一个指示符字段，1位字段。置'1'时表示传送的节目流映射当前是可用的。置'0'时表示传送的节目流映射还不可用，但它将是下一个生效的表。

**program_stream_map_version**：节目流映射版本字段 ，5位字段，表示整个节目流映射的版本号。一旦节目流映射的定义发生变化，该字段将递增1，并对32取模。在current_next_indicator为'1'时，该字段应该是当前适用的节目流映射的版本号；在current_next_indicator为'0'时，该字段应该是下一个适用的节目流映射的版本号。

**program_stream_info_length**：节目流信息长度字段，16位字段，指出紧跟在该字段后的描述符的总长度。

**marker_bit**：标记位字段，1位字段，取值为'1'。

**elementary_stream_map_length**：基本流映射长度字段，16位字段，指出在该节目流映射中的所有基本流信息的字节长度。它只包括stream_type、elementary_stream_id和elementary_stream_info_length字段。(这里注意一下，这里的基本流映射长度，他只包括他后面的指定的那几个定义字段的总和，即从从这个长度，我们可以知道后面他根了几种类型的流定义，因为一种流的这个定义字段：stream_type(1BYTE)、elementary_stream_id(1byte)和elementary_stream_info_length(2byte)字段总和为4个字节，所以用elementary_stream_map_length/4可以得到后面定义了几个流类型信息。)

**stream_type**：流类型字段，8位字段，根据表2-29规定了流的类型。该字段只能标志包含在PES分组中的基本流且取值不能为0x05。

根据国标GB28181中的定义可以知道

1、MPEG-4 视频流： 0x10；

2、H.264 视频流： 0x1B；

3、SVAC 视频流： 0x80；

4、G.711 音频流： 0x90；

5、G.722.1 音频流： 0x92；

6、G.723.1 音频流： 0x93；

7、G.729 音频流： 0x99；

8、SVAC音频流： 0x9B。

因为节目映射流字段只有在关键帧打包的时候，才会存在，所以如果要判断PS打包的流编码类型，就根据这个字段来判断。

**elementary_stream_id**，基本流标识字段，8位字段，指出该基本流所在PES分组的PES分组标题中stream_id字段的值。这个字段的定义，其中0x(C0~DF)指音频，0x(E0~EF)为视频。

**elmentary_stream_info_length**：基本流信息长度字段，16位字段，指出紧跟在该字段后的描述符的字节长度。(即这个类型的流描述长度。这个后面的字段后面的指定长度不在elementary_stream_map_length指定的范围类。)

CRC_32：CRC 32字段，32位字段，它包含CRC值以在处理完整个节目流映射后在附录A中定义的解码器寄存器产生0输出值。

对于这个字段的解析，我们需要取值0x000001BC的位串，指出节目流映射的开始，暂时不需要处理，读取Header Length直接跳过即可，如果需要解析流编码类型，必须详细解析这个字段。

#### 4、PES packet

 接下来我们来分析下PES包的内容：PES包=PES header+code raw data;

PES header bit:

![img](.\pic_resource\PS_note\PES_packet_info_bit.png)

PES 头部格式

 ![PES header](.\pic_resource\PS_note\PES_packet_info.png)

其中：

表2-18 Stream_id赋值

| **stream_id**       | 注   | 流 编 码                                          |
| ------------------- | ---- | ------------------------------------------------- |
| 1011 1100           | 1    | program_stream_map（0xBC）                        |
| 1011 1101           | 2    | private_stream_1(0xBD)                            |
| 1011 1110           |      | padding_stream(0xBE)                              |
| 1011 1111           | 3    | private_stream-2(0xBF)                            |
| 110x xxxx           |      | GB/T XXXX.3或GB/T AAAA.3音频流编号xxxx(0xC0~0xDF) |
| 1110 xxxx           |      | GB/T XXXX.2或GB/T AAAA.2视频流编号xxxx(0xE0~0xEF) |
| 1111 0000           | 3    | ECM_stream(0xF0)                                  |
| 1111 0001           | 3    | EMM_stream(0xF1)                                  |
| 1111 0010           | 5    | GB/T XXXX.1附录B或GB/T XXXX.6_DSMCC_stream(0xF2)  |
| 1111 0011           | 2    | ISO/IEC_13522_stream(0xF3)                        |
| 1111 0100           | 6    | ITU-T Rec. H.222.1类型A                           |
| 1111 0101           | 6    | ITU-T Rec. H.222.1类型B                           |
| 1111 0110           | 6    | ITU-T Rec. H.222.1类型C                           |
| 1111 0111           | 6    | ITU-T Rec. H.222.1类型D                           |
| 1111 1000           | 6    | ITU-T Rec. H.222.1类型E                           |
| 1111 1001           | 7    | ancillary_stream(0xF9)                            |
| 1111 1010…1111 1110 |      | 保留数据流                                        |
| 1111 1111           | 4    | program_stream_directory(0xFF)                    |
|                     |      |                                                   |

 注：

```
符号x表示值'0'或'1'均被允许且可产生相同的流类型。流号码由x的取值决定。注1 类型为program_stream_map的PES分组有唯一的语法，在2.5.4.1中作了规定。2 类型为private_stream_1和ISO/IEC_13352_stream的PES分组与GB/T XXXX.2及GB/T XXXX.3音频流服从相同的PES分组语法。3 类型为private_stream_2，ECM_stream和EMM_stream的PES分组与private_stream_1相似，除了在PES_packet_length字段后未规定语法。4 类型为program_stream_directory的PES分组有唯一的语法，在2.5.5中作了规定。5 类型为DSM_CC_stream的PES分组有唯一的语法，在GB/T XXXX.6中作了规定。6 stream_id与表2-29中的stream_type 0x09相关联。7 stream_id仅用于PES分组。PES分组在传输流中携带了来源于节目流或GB/T AAAA.1系统流的数据(参见2.4.3.7)。
```



PES分组中各字段的语义定义

packet_start_code_prefix：分组起始码前缀字段，24位代码，它和后面的stream_id构成了标识分组开始的分组起始码。它是一个值为'0000 0000 0000 0000 0000 0001' (0x000001)的位串。

stream_id：流标识字段，在节目流中，它规定了基本流的号码和类型。定义参见表2-18。在传输流中，它可以被设定为正确描述表2-18中定义的基本流类型的任何有效值。在传输流中，基本流类型在2.4.4的节目特定信息中作了规定。

PES_packet_length：PES分组长度字段，16位字段，指出了PES分组中跟在该字段后的字节数目。值为0表示PES分组长度要么没有规定要么没有限制。这种情况只允许出现在有效负载包含来源于传输流分组中某个视频基本流的字节的PES分组中。

PES_scrambling_control：PES加扰控制字段，2位字段，表示PES分组有效负载的加扰方式。当加扰发生在PES层，PES分组标题，如果有可选字段的话也包括在内，不应被加扰(参见2-19)。

PES_priority，PES优先级字段，1位字段，指示PES分组中有效负载的优先级。'1'表示PES分组中有效负载的优先级高于该字段为'0'的PES分组有效负载。多路复合器能使用该字段来区分安排基本流中数据的优先级。传输机制不应改动该字段。

data_alignment_indicator，数据对齐指示符字段，1位标志。置'1'时表示PES分组标题后紧跟着在2.6.10中的data_alignment_indicator所指出的视频起始码或音频同步字，如果有data_alignment_indicator描述符的话。若其值为'1'且无该描述符，则需要在表2-47和2-48中alignment_type '01'所表示的对齐。当值为'0'时，没有定义是否有任何此种的对齐。

copyright ：版权字段，1位字段。置'1'时表示相关PES分组有效负载的材料受到版权保护。当值为'0'时，没有定义该材料是否受到版权保护。2.6.24中描述的版权描述符与包含PES分组的基本流相关。若描述符作用于包含PES分组的材料，则版权标志被置为'1'。

original_or_copy：原始或拷贝字段，1位字段。置'1'时表示相关PES分组有效负载的内容是原始的；值为'0'表示相关PES分组有效负载的内容是一份拷贝。

PTS_DTS_flags：PTS DTS标志字段，2位字段。当值为'10'时，PTS字段应出现在PES分组标题中；当值为'11'时，PTS字段和DTS字段都应出现在PES分组标题中；当值为'00'时，PTS字段和DTS字段都不出现在PES分组标题中。值'01'是不允许的。

ESCR_flag：ESCR标志字段，1位标志。置'1'时表示ESCR基础和扩展字段出现在PES分组标题中；值为'0'表示没有ESCR字段。

ES_rate_flag：ES速率标志字段，1位标志。置'1'时表示ES_rate字段出现在PES分组标题中；值为'0'表示没有ES_rate字段。

DSM_trick_mode_flag：DSM特技方式标志字段，1位标志。置'1'时表示有8位特技方式字段；值为'0'表示没有该字段。

additional_copy_info_flag ：附加版权信息标志字段，1位标志。置'1'时表示有附加拷贝信息字段；值为'0'表示没有该字段。

PES_CRC_flag：PES CRC标志字段，1位标志。置'1'时表示CRC字段出现在PES分组标题中；值为'0'表示没有该字段。

PES_extension_flag：PES扩展标志字段，1位标志。置'1'时表示PES分组标题中有扩展字段；值为'0'表示没有该字段。

PES_header_data_length：PES标题数据长度字段，8位字段。指出包含在PES分组标题中的可选字段和任何填充字节所占用的总字节数。该字段之前的字节指出了有无可选字段。

marker_bit：标记位字段，值为'1'的1位字段。

PTS：展现时间戳字段，展现时间与解码时间的关系如下：PTS是一个编码在三个分离字段中的33位数字。它指出了基本流n的第k个展现单元在系统目标解码器中的展现时间tpn(k)。PTS的值以系统时钟频率的1/300(即90 kHz)为单位。展现时间由PTS根据式2-11计算而来。对编码展现时间戳频率的约束参见2.7.4。

*PTS*(*k*)＝((*system_clock_frequency*×*tpn*(*k*)) DIV 300) % 233      (2-11)

其中，tpn(k)是展现单元Pn(k)的展现时间。

对音频而言，若PES分组标题中有PTS，则它是指PES分组中开始的第一个存取单元。若PES分组中有音频存取单元的首字节，则有一个音频存取单元开始于该PES分组中。

对视频而言，若PES分组标题中有PTS，则它是指包含PES分组中开始的第一个画面起始码的存取单元。若PES分组中有画面起始码的首字节，则有一个画面起始码开始于该PES分组中。

对音频展现单元(PU)，low_delay序列中的视频PU以及B画面，展现时间tpn(k)应等于tdn(k)。

对于非low_delay中的I画面和P画面，在存取单元(AU) k和k'之间无解码不连续时，展现时间tpn(k)应等于下一个传输的I画面或P画面的解码时间tdn(k) (参见2.7.5)。若有解码不连续或流终止，则tpn(k)和tdn(k)之间的差别应与初始流一直延续，没有不连续也没有终止时完全相同。

注1： low_delay序列是low_delay标志被设置的视频序列(参见GB/T XXXX.2中的6.2.2.3)。

若音频中有滤波，则系统模型假定滤波不会导致延迟。因此，编码时PTS所涉及的采样与解码时PTS所涉及的采样是相同的。对于可伸缩编码，参见2.7.6。

DTS：解码时间戳字段，DTS是一个编码在三个分离字段中的33位数字。它指出了基本流n的第j个展现单元在系统目标解码器中的解码时间tdn(j)。DTS的值以系统时钟频率的1/300 (即90 kHz)为单位。解码时间由DTS根据式2-12计算而来：

*DTS*(*j*)＝((*system_clock_frequency*×tdn(j)) DIV 300) % 233       (2-12)

其中，tdn(j)是存取单元An(j)的解码时间。

对视频而言，若PES分组标题中有DTS，则它是指包含PES分组中开始的第一个画面起始码的存取单元。若PES分组中有画面起始码的首字节，则该画面起始码开始于该PES分组中。

对于可伸缩编码，参见2.7.6。

ESCR_base/ESCR_extension：ESCR字段，42位字段，分两部分编码。第一部分是一个长度为33位的字段，其值ESCR_base(i)由式2-14给出；第二部分是一个长度为9位的字段，其值ESCR_ext(i)由式2-15给出。ESCR字段指出了基本流中包含ESCR_base最后一个比特的字节到达PES-STD输出端的期望时间(参见2.5.2.4)。

特别地

*ESCR*(*i*)＝*ESCR_base*(*i*)×300＋*ESCR_ext*(*i*)                 (2-13)

其中：

*ESCR_base*(*i*)＝((*system_clock_frequency*×*t*(*i*)) DIV 300) % 233       (2-14)

*ESCR _ext*(*i*)＝((*system_clock_frequency*×*t*(*i*)) DIV 1) % 300        (2-15)

ESCR和ES_rate字段(参见下面紧接的语义)包含与PES流序列相关的时间信息。这些字段应满足2.7.3中定义的约束。

ES_rate：基本流速率字段，22位无符号整数。对于PES流而言，它指出了系统目标解码器接收PES分组的速率。该字段在它所属的PES分组以及同一个PES流的后续PES分组中一直有效，直到遇到一个新的ES_rate字段。该字段的值以50字节/秒为单位，且不能为0。该字段用于定义PES流的字节到达P-STD输入端的时间(参见2.5.2.4中的定义)。在各个PES分组中，编码在该字段中的值可能不同。

trick_mode_control：特技方式控制字段，3位字段。它表示作用于相关视频流的特技方式。对其它类型的基本流，该字段及其后5位的含义没有定义。trick_mode状态的定义参见2.4.2.3的特技模式部分。

当trick_mode状态为假时，对GB/T XXXX.2视频而言，解码过程输出渐进序列中每幅画面的次数N由repeat_first_field和top_field_first字段来规定。对GB/T AAAA.2视频而言，由序列标题决定。

对于隔行序列，当trick_mode状态为假时，对GB/T XXXX.2视频而言，次数N由repeat_first_field和progressive_frame字段来规定。

当trick_mode状态为真时，画面的播放次数依赖于值N。

表2-20 特技方式控制值

| 值           | 描 述  |
| ------------ | ------ |
| '000'        | 快进   |
| '001'        | 慢动作 |
| '010'        | 冻结帧 |
| '011'        | 快倒   |
| '100'        | 慢倒   |
| '101'－'111' | 保留   |

 

当该字段值发生变化或特技模式操作停止时，可能会出现下列情况的任意组合：

l      时基不连续；

l      解码不连续；

l      连续性计数器不连续；

在特技模式的情况下，解码和展现的非标准速度可能会导致视频基本流数据中定义的某些字段值不正确。同样，片段结构的语义约束也可能无效。这些例外所涉及的视频语法元素为：

l      bit_rate;

l      vbv_delay;

l      repeat_first_field;

l      v_axis_positive;

l      field_sequence;

l      subcarrier;

l      burst_amplitude;

l      subcarrier_phase;

在特技模式中，解码器不应该依赖于编码在这些字段中的值。

标准并不要求解码器能解码trick_mode_control字段。但是，能解码该字段的解码器应能满足以下标准要求。

fast forword，快进

trick_mode_control字段中的值'000'。当该值出现时，它表示一个快进视频流并定义了PES分组标题中后续5位的含义。intra_slice_refresh位可以被设定为'1'以指出可能有丢失的宏块。解码器可以用前一个解码画面中相同位置的宏块来代替。表2-21中定义的field_id字段，表示应该显示哪个或哪些字段。frequency_truncation字段指出了可能包括的一个系数受限集合。该字段值的含义如表2-22所示。

slow motion，慢动作

trick_mode_control字段中的值'001'。当该值出现时，它表示一个慢动作视频流，并定义了PES分组标题中后续5个比特的含义。对渐进序列而言，该画面应被显示N×rep_cntrl时间，其中N定义如上。

对GB/T AAAA.2视频和GB/T XXXX.2视频渐进序列而言，该画面应被显示N×rep_cntrl时间，其中N定义如上。

对GB/T XXXX.2隔行序列而言，该画面应被显示N×rep_cntrl时间。若该画面是一个帧画面，则待显示的第一个字段在top_field_first为1时应该是顶字段，在top_field_first为0时，应该是底字段(参见GB/T XXXX.2)。该字段被显示N×rep_cntrl / 2时间。该画面的其它字段被显示N－N×rep_cntrl / 2时间。

freeze frame，冻结帧

trick_mode_control字段中的值'010'。当该值出现时，它表示冻结帧视频流，并定义了PES分组标题中后续5位的含义。表2-21中定义的field_id字段，表示应该显示哪个(些)字段。field_id字段指出了包含该字段的PES分组中开始的第一个视频存取单元，除非该PES分组包含0个有效负载字节。在后一种情况下，field_id字段指出了最近的前一个视频存取单元。

fast reverse，快倒

trick_mode_control字段中的值'011'。当该值出现时，它表示一个快倒视频流并定义了PES分组标题中后续5位的含义。intra_slice_refresh位可以被设定为'1'以指出可能有丢失的宏块。解码器可以用前一个解码画面中相同位置的宏块来代替。表2-21中定义的field_id字段，表示应该显示哪个或哪些字段。frequency_truncation字段指出了可能包括的一个系数受限集合。该字段值的含义如表2-22“系数选择值”所示。

slow reverse，慢倒

```
trick_mode_control字段中的值'100'。当该值出现时，它表示一个慢倒视频流并定义了PES分组标题中后续5位的含义。对GB/T AAAA.2视频和GB/T XXXX.2视频渐进序列而言，该画面应被显示N×rep_cntrl时间，其中N定义如上。
对GB/T XXXX.2隔行序列而言，该画面应被显示N×rep_cntrl时间。若该画面是一个帧画面，则待显示的第一个字段在top_field_first为1时应该是底字段，在top_field_first为'0'时，应该是顶字段(参见GB/T XXXX.2)。该字段被显示N×rep_cntrl/2时间。该画面的其它字段被显示N－N×rep_cntrl / 2时间。
```

field_id:字段标识字段，2位字段，表示应该显示哪个(些)字段。根据表2-21对其进行编码。

表2-21 field_id字段控制值

| 值   | 描 述          |
| ---- | -------------- |
| '00' | 仅自顶向下播放 |
| '01' | 仅自底向上播放 |
| '10' | 播放所有帧     |
| '11' | 保留           |

 

intra_slice_refresh，片内参考字段，1位标志。置'1'时表示PES分组的视频数据编码片中可能有丢失的宏块；置'0'时，表示上述情况可能不出现。更多的信息可参见GB/T XXXX.2。解码器可以用前一个解码画面中同一个位置的宏块来代替丢失的宏块。

frequency_truncation：频率截断字段，2位字段。指出在对PES分组中数据进行编码时可能用到受限系数集合。其值定义于表2-22。

表2-22 系数选择值

| 值   | 描述              |
| ---- | ----------------- |
| '00' | 仅DC系数非0       |
| '01' | 仅前三个系数非0   |
| '10' | 仅前六个系数非0   |
| '11' | 所有系数均可能非0 |

 

rep_cntrl ：显示次数控制字段，5位字段，指出隔行画面中每一字段的显示次数或渐进画面显示次数。对隔行画面而言，顶字段或底字段是否应首先显示是视频序列标题中trick_mode_control字段和top_field_first字段的功能。该字段值不能为'0'。

additional_copy_info：附加版权信息字段，7位字段，包含与版权信息有关的专用数据。

previous_PES_packet_CRC：前PES分组CRC字段，16位字段。在对前一个PES分组(不包括该PES分组的标题)进行处理后，该字段包含一个在解码器的16个寄存器中生成0输出的CRC值。该CRC值与附录A中所定义的相类似，但具有以下多项式：

x16＋x12＋x5＋1

注2： 该CRC值是为了用于网络维护，例如将有间隙性错误的源隔离开来，而不是为了供基本流解码器使用。它仅用于计算数据字节，因为在传输过程中PES分组标题数据可能被修改。

PES_private_data_flag：PES专用数据标志字段，1位标志。置'1'时表示PES分组标题中包含专用数据；置'0'时表示PES分组标题中无专用数据。

pack_header_field_flag：包标题字段标志字段，1位标志。置'1'时表示PES分组标题中有GB/T AAAA.2包标题或节目流包标题。若该字段在包含于节目流中的PES分组中，其值应为'0'。在传输流中，当值为'0'时表示PES标题中无包标题。

program_packet_sequence_counter_flag：节目分组序列计数标志字段，1位标志。值为'1'时表示PES分组有program_packet_sequence_counter，MPEG1_MPEG2_identifier和original_stuff_length字段。值为'0'时表示PES分组标题中无这些字段。

P-STD_buffer_flag：P-STD缓冲区标志字段，1位标志。置'1'时表示PES分组标题中有P-STD_buffer_scale和P-STD_buffer_size字段。值为'0'时表示PES标题中无这些字段。

PES_extension_flag_2：PES扩展标志字段，1位标志，置'1'时表示有PES_extension_field_length及相关字段。

PES_private_data：PES专用数据字段，16位字段。包含专用数据。这些数据与其前后的字段组合在一起时，不能与packet_start_code_prefix (0x000001)冲突。

pack_field_length：包字段长度字段，8位字段。表示pack_header_field()以字节为单位时的长度。

program_packet_sequence_counter：节目分组序列计数字段，7位字段。它是一个可选的计数器，随着来自于节目流或GB/T AAAA.1流的每一个后续的PES分组或传输流中具有单个节目定义的PES分组而递增，以提供与连续性计数器(参见2.4.3.2)相似的功能。它能用于检索节目流或原始GB/T AAAA.1流中的初始PES分组序列。该计数器在达到最大值后回卷为0。PES分组不能出现重复。因此，复合节目中任何两个连续的PES分组不应具有相同的program_packet_sequence_counter值。

MPEG1_MPEG2_identifier：MPEG1 MPEG2标识符字段，1位标志。置'1'时表示PES分组携带的信息来自于GB/T AAAA.1流；置'0'时表示PES分组携带的信息来自于节目流。

original_stuff_length：初始填充长度字段，6位字段。指定用于初始GB/T XXXX.1分组标题或初始GB/T AAAA.1分组标题中的填充字节数。

P-STD_ buffer_scale：P-STD缓冲区比例字段，1位字段。仅当该PES分组包含于节目流中时才有意义。它指出了用来解释后续P-STD_buffer_size字段的比例因子。若前面的stream_id表示一个音频流，该字段值应为'0'；若前面的stream_id表示一个视频流，该字段值应为'1'。对于所有的其它流类型，其值可以为'0'或'1'。

P-STD_buffer_size：P-STD缓冲区大小字段，13位无符号整数。仅当该PES分组包含于节目流中时才有意义。它定义了P-STD输入缓冲区的大小BSn。若P-STD_ buffer_scale的值为'0'，那么P-STD_buffer_size以128字节为单位来度量缓冲区的大小。若P-STD_buffer_scale的值为'1'，那么P-STD_buffer_size以1024字节为单位来度量缓冲区的大小。因此：

if ( *P-STD_buffer_scale* == 0)

*BSn*＝*P-STD_buffer_size*×128                                  (2-16)

else

*BSn*＝*P-STD_buffer_size*×1024                                 (2-17)

当该字段被GB/T XXXX.1系统目标解码器收到后，其编码值立即生效。

PES_extension_field_length：PES扩展字段长度字段，7位字段。指出了跟在该字段之后在PES扩展字段中直到且包括任何保留字节的数据的字节长度。

stuffing_byte：填充字节字段，8位字段，其值恒定为'1111 1111'。可以由编码器插入以满足通道的需求等。解码器丢弃该字段。一个PES分组标题中只能出现32个填充字节。

PES_packet_data_byte：PES分组数据字节字段，该字段应该是来自于由分组的stream_id或PID所指定的基本流的连续数据字节。当基本流数据符合GB/T XXXX.2或GB/T XXXX.3时，该字段应该是与本标准的字节相对齐的字节。基本流的字节序应得到保持。该字段的字节数N由PES_packet_length字段规定。N应等于PES_packet_length减去在PES_packet_length字段的最后一个字节与第一个 PES_packet_data_byte间的字节数。

padding_byte，填料字节字段，8位字段，其值恒定为'1111 1111'。该字段被解码器丢弃。



### 参考：

https://www.cnblogs.com/lihaiping/p/4181607.html

https://www.cnblogs.com/lihaiping/p/12316315.html

http://blog.csdn.net/wangjiannuaa/article/details/7679486

http://blog.csdn.net/chen495810242/article/details/39207305

http://blog.csdn.net/chen495810242/article/details/39207305