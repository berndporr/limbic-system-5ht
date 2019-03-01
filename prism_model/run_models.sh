echo "Starting experiment 1:" | tee results.txt
prism final_5ht.prism prop2_new.pctl -const delay=15,reward_unseen_speed=2,speed_uncertainty=0 | tee -a results.txt
echo "Starting experiment 2:" | tee -a results.txt
prism final_5ht.prism prop2_new.pctl -const delay=10,reward_unseen_speed=2,speed_uncertainty=0 | tee -a results.txt
echo "Starting experiment 3:" | tee -a results.txt
prism final_5ht.prism prop2_new.pctl -const delay=15,reward_unseen_speed=120,speed_uncertainty=12 | tee -a results.txt
echo "Starting experiment 4:" | tee -a results.txt
prism final_5ht.prism prop2_new.pctl -const delay=10,reward_unseen_speed=120,speed_uncertainty=12 | tee -a results.txt
echo "Starting experiment 5:" | tee -a results.txt
prism final_5ht.prism prop2_new.pctl -const delay=15,reward_unseen_speed=40,speed_uncertainty=4 | tee -a results.txt
echo "Starting experiment 6:" | tee -a results.txt
prism final_5ht.prism prop2_new.pctl -const delay=10,reward_unseen_speed=40,speed_uncertainty=4 | tee -a results.txt
echo "Starting experiment 7:" | tee -a results.txt
prism final_5ht.prism prop2_new.pctl -const delay=15,reward_unseen_speed=160,speed_uncertainty=20 | tee -a results.txt
echo "Starting experiment 8:" | tee -a results.txt
prism final_5ht.prism prop2_new.pctl -const delay=10,reward_unseen_speed=160,speed_uncertainty=20 | tee -a results.txt
echo "Program finished" | tee -a results.txt
