# MediaServer
Media Server implementing Modern HTTP  MPEG-DASH and HLS protocols

### OBJECTIVE

The goal of the project is to implement a **Media server** compatible with both MPEG Dynamic Adaptive Streaming over HTTP ( DASH) and HTTP Live streaming(HLS). 

### PROJECT DETAILS/PROPOSED METHODOLOGY

The following is our step-by-step plan:

For both the methods the common structure for the Media server/client is :

### Server :     

* Segment the media or streaming buffer to make a playlist using ffmpeg[[3]](#bookmark=kix.87wymmvwged) ( Handle audio and video stream appropriately). 

* Add appropriate signature and track information and append the parsable format as mentioned in RFC [[1]](#bookmark=kix.l4nu6e3zcb28) for HLS and RFC [[2]](#bookmark=kix.se2mrw268qcg) for DASH.

* Encode and encrypt the segments (END-to-END) and send securely over HTTP.

### Client :

* Decrpyt the segments upon receiving them ( Handle audio and video stream appropriately). 

* Parse the receiving segments and use header information to recreate the proper sequence.

* Decode the sequence and recreate the playlist.

* Read from the buffer and stream it using openCV [[4]](#bookmark=id.s8kh5y53ylvx).

### Other:

1. Also implement protocol specific logging, adaptive features etc.

2. Evaluate performance metrics to decide which method is better with respect to which parameters.

 

### LIST OF MODULES AND DISTRIBUTION

Siddhartha Mishra

* DASH :  Implementation of the segmentation, encryption and specific features and working prototype as mentioned in RFC.

* Client side : Parsing segment and decoding them to recreate playlists on client side. 

Raaghav R.

* HLS : Implementation of the segmentation, encryption and specific features and working prototype as mentioned in RFC.

* Client side : Render streaming buffer as received in order on display using OpenCV. 

     	

 

### DELIVERABLES

* Implementation of MPEG-DASH and HLS protocol and a working prototype of the Media server and client.

* Performance comparison using visualizable graphs for both protocols based on the following parameters:

    * Streaming performance : Startup delay, jitter, latency, re buffer duration etc.

    * Video Quality : Resolution , packets lossed , Bit rate, frame rate etc. 

### LIST OF TOOLS, SOFTWARES AND OTHER REFERENCES:

[1][ https://tools.ietf.org/html/rfc8216](https://tools.ietf.org/html/rfc8216)( RFC for HLS)

[2] [https://tools.ietf.org/html/rfc6983](https://tools.ietf.org/html/rfc6983) (RFC for MPEG-DASH)

[3] [https://ffmpeg.org](https://ffmpeg.org) Used to create ffmpeg segments of a continuous stream using a c ommon encoder so that it can be decoded properly on the client side.

[4] [https://opencv.org](https://opencv.org) To render video on client side stream from received buffers over network from media server.

