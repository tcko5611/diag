
.model mynch nmos level=72
.model mypch pmos level=72

.subckt inv a y vdd
mp1 y a vdd vdd mypch l=200e-9 nfin=2
mn1 y a 0   0   mynch l=200e-9 nfin=2
.ends

.subckt xor2 in1 in2 out vdd
xinv1 in1 in1b vdd inv
xinv2 in2 in2b vdd inv
mp1 y1  in1  vdd vdd mypch l=200e-9 nfin=2
mp2 out in2b y1  vdd mypch l=200e-9 nfin=2
mp3 y2  in1b vdd vdd mypch l=200e-9 nfin=2
mp4 out in2  y2  vdd mypch l=200e-9 nfin=2
mn1 out in1  y3  0   mynch l=200e-9 nfin=2
mn2 y3  in2  0   0   mynch l=200e-9 nfin=2
mn3 out in1b y4  0   mynch l=200e-9 nfin=2
mn4 y4  in2b 0   0   mynch l=200e-9 nfin=2
.ends

.subckt mux21 a b s y vdd
xiv1 s sb vdd inv
xnand1 a sb  x1 vdd nand
xnand2 b s   x2 vdd nand
xnand3 x1 x2 y  vdd nand
.ends

.subckt nand a b y vdd
mp1 y a vdd vdd mypch w=10n nfin=4
mp2 y b vdd vdd mypch w=10n nfin=4
mn1 y a x 0 mynch w=10n nfin=2
mn2 x b 0 0 mynch w=10n nfin=2
.ends

.subckt srhw a b y vdd
x1 a b y vdd nand
.ends

.subckt sm1 a b y alarm ten atest btest ytest vdd
xmuxa a atest ten a_smi vdd mux21
xmuxb b btest ten b_smi vdd mux21
xmuxy y ytest ten y_smi vdd mux21
x1 a_smi b_smi y_sm vdd nand
x2 y_smi y_sm alarm vdd xor2
*xmuxalarm alarm_sm 0 ten alarm vdd mux21
.ends

.subckt latent_sm a b y_sm latent_alarm vdd
x1 a b y_latent vdd nand
x2 y_sm y_latent latent_alarm vdd xor2
.ends

.subckt lbist alarm ten atest btest ytest latent_alarm vdd
vatest atest 0 pulse(0 vdd 20n 10p 10p 100n 200n)
vbtest btest 0 pulse(0 vdd 20n 10p 10p 200n 400n)
vytest ytest 0 pulse(0 vdd 20n 10p 10p 400n 800n)
xnand atest btest yy vdd nand
xxor1 yy    ytest zz vdd xor2
xxor2 alarm zz    la vdd xor2
xmux  0 la ten latent_alarm vdd mux21
.ends

.param vdd=1.0
vvdd vdd 0 dc=vdd
va a 0 pulse(0 vdd 10n 10p 10p 100n 200n)
vb b 0 pulse(0 vdd 10n 10p 10p 200n 400n)
*xinv1 a   a_b vdd  inv
*xinv2 a_b aa  vdd inv
*xinv3 b   b_b vdd inv
*xinv4 b_b bb vdd inv

vten ten 0 pwl 0 0 5n 0

XSRHW a b y vdd srhw
XSM   a b y alarm ten atest btest ytest vdd sm1
XLBIST alarm ten atest btest ytest latent_alarm vdd lbist
*XSMLATENT a b y_sm latent_alarm vdd latent_sm

*xmuxtest a b s muxout vdd mux21
*vs s 0 dc=1

.tran 1n 1u
.probe v(*) 
.opt xa_cmd="probe_waveform_voltage -v * -port 1 -level 99"
.opt xa_cmd="set_sim_level 7"

.param offset=50n
.meas out100n max v(y) from=100n-offset to=100n
.opt xa_cmd="set_sim_stop -check meas -meas_name out100n -max vdd/2 -error 0 -id fo1"
.meas out200n max v(y) from=200n-offset to=200n
.opt xa_cmd="set_sim_stop -check meas -meas_name out200n -min vdd/2 -error 0 -id fo2"

.meas out300n max v(y) from=300n-offset to=300n
.opt xa_cmd="set_sim_stop -check meas -meas_name out300n -min vdd/2 -error 0 -id fo3"
.meas out400n max v(y) from=400n-offset to=400n
.opt xa_cmd="set_sim_stop -check meas -meas_name out400n -min vdd/2 -error 0 -id fo4"


.meas alarm1 max v(alarm) from=100n-offset to=100n
.meas alarm2 max v(alarm) from=200n-offset to=200n
.meas alarm3 max v(alarm) from=300n-offset to=300n
.meas alarm4 max v(alarm) from=400n-offset to=400n
.opt xa_cmd="set_sim_stop -check meas -meas_name alarm1 -max vdd/2 -error 0 -id sm1"
.opt xa_cmd="set_sim_stop -check meas -meas_name alarm2 -max vdd/2 -error 0 -id sm2"
.opt xa_cmd="set_sim_stop -check meas -meas_name alarm3 -max vdd/2 -error 0 -id sm3"
.opt xa_cmd="set_sim_stop -check meas -meas_name alarm4 -max vdd/2 -error 0 -id sm4"

.meas latent_alarm1 max v(latent_alarm) from=100n-offset to=100n
.meas latent_alarm2 max v(latent_alarm) from=200n-offset to=200n
.meas latent_alarm3 max v(latent_alarm) from=300n-offset to=300n
.meas latent_alarm4 max v(latent_alarm) from=400n-offset to=400n
.opt xa_cmd="set_sim_stop -check meas -meas_name latent_alarm1 -max vdd/2 -error 0 -id lsm1"
.opt xa_cmd="set_sim_stop -check meas -meas_name latent_alarm2 -max vdd/2 -error 0 -id lsm2"
.opt xa_cmd="set_sim_stop -check meas -meas_name latent_alarm3 -max vdd/2 -error 0 -id lsm3"
.opt xa_cmd="set_sim_stop -check meas -meas_name latent_alarm4 -max vdd/2 -error 0 -id lsm4"

.end
