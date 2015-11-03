#!/bin/sh

datasize=(10000 20000 30000 40000 50000 60000 70000 80000 90000 100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000)

#echo "$i:@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" >> info.txt
#
#i=0
#while [ $i -lt ${#datasize[@]} ]
#do
#	echo "ds:${datasize[$i]}" >> info.txt
#	i=$(($i + 1))
#done


#build mode 2
i=0
while [ $i -lt ${#datasize[@]} ] 
do
	./umadclient --task index --runOption build --buildMode 2 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init ${datasize[$i]} --F ${datasize[$i]} --s ${datasize[$i]} --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l 10 --a 2 --i 2 --y 0.2 --res indexbuildingresult_2 --indexName vector_index_balanced_fft_2 --searchMode 1 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance
	i=$(($i+1))
done


#build mode 0
i=0
while [ $i -lt ${#datasize[@]} ]
do
	./umadclient --task index --runOption build --buildMode 0 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init ${datasize[$i]} --F ${datasize[$i]} --s ${datasize[$i]} --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l 10 --a 2 --i 2 --y 0.2 --res indexbuildingresult_0 --indexName vector_index_balanced_fft_0 --searchMode 1 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance
	i=$(($i+1))
done	


i=1
while [ $i -lt 10 ]
do	
#searchMode 0

	./umadclient --task index --runOption search --searchMode 0 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_0 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance


#searchMode 1

	./umadclient --task index --runOption search --searchMode 1 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_1 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance


#searchMode 3

	./umadclient --task index --runOption search --searchMode 3 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_3 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance



#searchMode 4

	./umadclient --task index --runOption search --searchMode 4 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_4 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance

	i=$(($i+1))
done



i=10
while [ $i -lt 100 ]
do	
#searchMode 0

	./umadclient --task index --runOption search --searchMode 0 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_0 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance


#searchMode 1

	./umadclient --task index --runOption search --searchMode 1 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_1 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance


#searchMode 3

	./umadclient --task index --runOption search --searchMode 3 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_3 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance



#searchMode 4

	./umadclient --task index --runOption search --searchMode 4 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_4 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance

	i=$(($i+10))
done


#i=100
#
#	./umadclient --task index --runOption search --searchMode 4 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_4 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance




i=100
while [ $i -le 1000 ]
do	
#searchMode 0

	./umadclient --task index --runOption search --searchMode 0 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_0 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance


#searchMode 1

	./umadclient --task index --runOption search --searchMode 1 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_1 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance


#searchMode 3

	./umadclient --task index --runOption search --searchMode 3 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_3 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance



#searchMode 4

	./umadclient --task index --runOption search --searchMode 4 --n uniformvector-20dim-1m.txt --v 2 --sete 10000 --setc 50 --f 3 --fftscale 100 --psm fft --dpm balanced  --m 100 --t vector --init 1000000 --F 1000000 --s 1000000 --b 0 --frag 6 --dim 20 --r 2 --e MVPIndex --fftopt 0 --q uniformvector-20dim-1m.txt --fq 0 --l "$i" --a 2 --i 2 --y 0.2 --res searchresult_4 --buildMode 0 --indexName vector_index_balanced_fft_1000000_0 --maxR 2.0 --midP 0.5 --putinfo 1 --cacheHeight 10 --tsThreadNum 30 --csThreadNum 25 --bThreadNum 32 --cacheAll 1 --tR 0.25 --sop Sequence --pbop Balance

	i=$(($i+50))
done
