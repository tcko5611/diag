
cat tb1.xa/tb1.xa.FM001.dangerous_detected tb1.xa/tb1.xa.FM002.dangerous_detected > tb1.xa.FMall.dangerous_detected

cat tb2.xa/tb2.xa.FM001.dangerous_undetected tb2.xa/tb2.xa.FM001.safe_undetected \
    tb2.xa/tb2.xa.FM002.dangerous_undetected tb2.xa/tb2.xa.FM002.safe_undetected \
    > tb2.xa.FMall.undetected

sleep 3

gen_dual_fault.sh tb1.xa.FMall.dangerous_detected tb2.xa.FMall.undetected > step3.dual_fault_list
