编解码技术：I帧与IDR帧的区别总结


DR（Instantaneous Decoding Refresh）--即时解码刷新。

I帧与IDR帧的相同点在于：



1、I和IDR帧都是使用帧内预测的；

2、都是同一个东西而已，在编码和解码中为了方便，要首个I帧和其他I帧区别开，所以才把第一个首个I帧叫IDR，这样就方便控制编码和解码流程。



I帧与IDR帧的不同点在于：



1、IDR帧的作用是立刻刷新，使错误不致传播，从IDR帧开始，重新算一个新的序列开始编码。而I帧不具有随机访问的能力，这个功能是由IDR承担；

2、IDR会导致DPB（DecodedPictureBuffer 参考帧列表——这是关键所在）清空，而I不会；

3、IDR图像一定是I图像，但I图像不一定是IDR图像；

4、一个序列中可以有很多的I图像，I图像之后的图像可以引用I图像之间的图像做运动参考；对于IDR帧来说，在IDR帧之后的所有帧都不能引用任何IDR帧之前的帧的内容，与此相反，对于普通的I-帧来说，位于其之后的B-和P-帧可以引用位于普通I-帧之前的I-帧；

5、从随机存取的视频流中，播放器永远可以从一个IDR帧播放，因为在它之后没有任何帧引用之前的帧。但是，不能在一个没有IDR帧的视频中从任意点开始播放，因为后面的帧总是会引用前面的帧。





https://blog.csdn.net/aflyeaglenku/article/details/106895104

##### 各种 NALU-Type

下图所有H.265的NALU
![在这里插入图片描述](.\pic_resource\H_26x_note\ref123456.png)

##### 关于TRAIL

Trailing pictures包括TSA、STSA和TRAIL。
在Trailing pictures中，除了TSA和STSA之外，其他的都定义为TRAIL帧。

##### 关于TSA和STSA

这两种是H265中引入的新结构。
Temporal sub-layer access picture随机接入点后按解码和输出顺序的图像。
TSA(temporal sublayer access)
STSA(stepwise temporal sublayer access)
1.3 关于RADL和RASL
Leading picture 包括RASL和RADL。
RADL(random access decodable leading)：在随机接入点后，可以按顺序解码的图像。不依赖IRAP前码流信息的前置图像。即IRAP介入，其RADL图像可以正确解码。
RASL(random access skipped leading) 因为包含帧间信息而不能解码的图像–会被丢弃。依赖IRAP前码流信息的前置图像。即IRAP介入，其RADL图像不能正确解码

这是两种GOP间的图像类型。如果解码器从某个CRA随机接入，则按照显示顺序的后面几帧数据由于缺少参考帧而不能解码，这些图像将被解码器抛弃，即skip leading。而对于没有从当前CRA接入的数据，这些图像可以被正常解码显示，因此称为decodable leading。由于这些数据是有可能舍弃的，因此其他图像（trailing pictures）不能参考这些数据，否则万一这些图像被舍弃，将会有更多的图像受其影响而不能正常解码。

举例说明：
假设视频序列的显示顺序为①，这是一个完整的GOP，解码顺序为②
①I B B P B B P B B P
②I P B B P B B P B B
在H.264中，第一个I帧为IDR，GOP为闭合结构，因此两个GOP组成视频的结构为
I B B P B B P B B PI B B P B B P B B P（显示顺序）
I P B B P B B P B BI P B B P B B P B B（解码顺序）
而在HEVC中，两个I帧为CRA，GOP为开放结构，因此GOP的结构为：
I B B P B B P B B PB B I B B P B B P B（显示顺序）
I P B B P B B P B B I B BP B B P B B…（解码顺序）
两个黄色的B帧表示的是按照解码顺序在CRA之后，该GOP内参考的前一个GOP进行编码的图像。这样便很容易得知，如果选择在第二个CRA进行随机接入，这两个黄色的B帧将会由于没有参考帧无法解码而被舍弃。这两个黄色的B帧即RASP。如果没有选择这个CRA进行随机接入，这两个黄色B帧将可以顺利解码，即成为RADP。
对于BLA，情况也是类似的。由于出现码流拼接，第二段码流的CRA之后的B也会因为没有参考帧无法解码而丢弃。很容易理解，此时缓存中的参考帧数据还来自上一段码流，跟当前码流没关系，当然不能用作B的参考了。
之余HEVC这么设计的目的，我觉得应该是为了编码效率考虑的。因为B帧的压缩比相对是最高的，引入这种设计可以在不影响随机存取性能的前提下，尽可能增大B帧的比重，提高整体压缩编码的性能。

以下是一些国外研究者在论坛中对这个问题的一些讨论，可以拿来做一下参考：
forum.doom9.org/archive/index.php/t-105129.html

##### 关于BLA

BLA(broken link access)
BLA只是CRA在视频流切换情况下的一种特例。视频流在某个RAP上要求切换到另一个视频流继续解码，则直接将该CRA同另一个视频流中的接入CRA连接，后者便是BLA。由于BLA之前解码到缓存的视频流与当前视频流无关，因此其特性类似于直接从该点进行随机存取后的CRA。

##### 关于IDR

IDR：Instantaneous Decoding Refresh
是在H.264中定义的结构。在H.264中，IDR帧一定是I帧，而且一定是GOP的开始，也是H.264 GOP的关键帧。但是反过来却不成立，I帧不一定是IDR帧。GOP的长度不是定死不变的，在H.264的编码器中，如果判定场景发生变化，那么即使不到原定GOP的末尾，也会在这个位置加入一个IDR，作为新一个GOP的开始。此时这个GOP的长度就被缩小了。

##### 关于CRA和GOP

GOP：Group of Pictures
CRA：clean random access

GOP是图像组（Group of Pictures）的意思，表示编码的视频序列分成了一组一组的有序的帧的集合进行编码。每个GOP一定是以一个I帧开始的，但是却不一定指代的是两个I帧之间的距离。因为一个GOP内可能包含几个I帧，只有第一个I帧（也就是第一帧）才是关键帧。在程序cfg中，GOP的长度和两个I帧的距离也是两个不同参数指定的（如IntraPeriod和GOP Size或者类似的参数）。所以，两个I帧的间距不可能大于GOP的长度，一般情况是更小的。
GOP有封闭式（Open GOP）和开放式（Closed GOP）两种。H264只有封闭式，H265两种都有。
H264封闭式GOP: H264中单个GOP内部帧解码不会依赖其它GOP，即每个GOP都可独立解码，因此成为是“封闭”的。每个GOP都可独立解码的关键点就是IDR帧的强制刷新起了阻隔作用，避免错误帧相互影响，所以H264中GOP开始的第一帧必须为IDR关键帧，IDR为I帧的一种采用帧内编码无参考方式，但I帧不一定是IDR帧，普通I帧会参考前边的IDR，在RTC中，用的最多的就是IDR关键帧。
但是在HEVC中，GOP的结构发生了变化，采用了“开放”的结构，在解码过程过可能会参考其他GOP的数据。这时，一个GOP的起始帧命名为CRA, clean random access,同样采用帧内编码，但是这个GOP内的帧间编码帧可以越过CRA参考前一个GOP的数据，这便是GOP的open。

##### 关于IRAP

Intra Random Access Point:帧内随机接入点
为了满足压缩视频流的随机介入需求，如电视频道的切换、视频播放中的快进、拖曳等，一段视频流中通常会间隔地存在一些随机介入点。从IRAP开始，后续视频流（播放顺序在IRAP后的图像）可以独立正确解码，无需参考IRAP前面的视频信息。IRAP后的第一幅解码图像被称为IRAP图像。IRAP图像只包含帧内编码片，不采用帧间预测技术，可以独立解码。
IRAP相当于H264的IDR帧。
IRAP的前置（Leading）图像：解码顺序在IRAP图像后而播放顺序在其前面的图像。
IRAP的后置（Trailing）图像：播放顺序在IRAP图像后（解码顺序必在其后）的图像。

IRAP的分类：BLA图像，IDR图像，和CRA图像。
IDR图像在H.264中作为IRAP图像使用，IDR图像要求其前置图像必须是RADL图像，即IDR图像及其后续码流可以不依赖该IDR图像前的视频流信息进行独立解码。
CRA图像允许其前置图像识RASL图像，允许参考CRA图像前的视频流可以使RASL图像获得更高的编码效率，当直接从CRA图像介入，其RASL图像无法正常解码。当以CRA图像开始的码流的一部分也属于另一段码流时，其RASL肯定不能正常解码。为了方便，将CRA定义为BLA图像，明确其RASL图像不需要解码。
CVS（Coded Video Sequence）：由IRAP所分成的相对独立的区域，且两个相邻IRAP图像的NORaslOutFlag=1的视频流。
CVS包含的编码图像组相当于早期的GOP，是一个相对可以独立解码的单位。编码过程通常以CVS为单位设置固定的时域编码结构。不同CVS内的视频压缩数据不会相互参考的称为封闭式GOP。

##### VPS

Video Parameter Sets

##### SPS

Sequence Parameter Sets

##### PPS

Picture Parameter Sets

##### AUD

Access unit delimiter

##### EOS

End of sequence

##### EOB

End of bitstream

##### FD

Filler data

##### SEI

Supplemental enhancement information

SEI_PREFIX
SEI_SUFFIX

有另一位博主的博客写的很专业，以供参考：
https://www.cnblogs.com/DwyaneTalk/p/5711430.html