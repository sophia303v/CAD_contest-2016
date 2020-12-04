# CAD_contest-2016

  Static timing analysis (STA) is a method of validating the timing performance of a design by checking all possible paths for timing violations.  STA breaks a design down into timing paths, calculates the signal propagation delay along each path, and checks for violations of timing constraints inside the design and at the input/output interface. We (Me and 3 friends) attended the CAD contest in 2016 and tried to address this problem.

#### An example of true path in a circuit.

![](https://i.imgur.com/FpGKmYN.png)


-----

### Goal : 
  The goal is to develop an efficient STA program to find the true paths under a given multi-core computing environment.
  

### The Results:
The fourth rank among competitors
![](https://i.imgur.com/T8Wpgjm.png)

### Terminolgy:
There are some terms used in this project, which can be referenced here.
https://cad-contest-2016.el.cycu.edu.tw/Problem_D/default.html
* True path
* floating-mode
* ...etc


### The conditions in given circuit benchmark
We were given information of different combinational circuits, each containing three kinds of logic gates, NAND, NOR and NOT.
all combinational circuits
* every gate delay is 1ns
* every gate has its controlling value
* the wire delay is zero
* The input files contain a Verilog gate-level netlist and its associated Verilog model.

### Our Algorithm and DataStructure.

#### Algo.



#### Data Structure.

Static timing analysis 是電路設計上一種驗證效能的方法，目標是驗證電路的時間表現。
* static，指的是相對 dynamic的方式，dynamic就是指用simulation tool, 直接去決定電路的完整情況，但這樣 dynamic 的方式是很花費時間的，static 的方式就相對快速，因為他不需要模擬整個完整電路一步步邏輯運作的過程。
* timing analysis, 是希望我們去分析他要獲得這個電路最後期望的邏輯結果，需要多少時間。

-

每個 gate 會被給定一些限制條件，除了這個 gate 本身的邏輯意義，還有比如經過這個gate需要多少時間，不同 gate 會有不同的作用時間，因此一個 gate 的不同 input  訊號抵達的時間就會不相同。
那剛剛我們說的，這樣的時間是可以被優化的，因為在一個電路的設計中，其實可能有很多的邏輯相連之後會有跟其他邏輯gate 產生重複的作用，另外，邏輯gate當中也會有controlling value的問題，比如一個 AND gate  如果有一個 zero值，他的輸出就一定是 zero。那這個 gate 的 zero input 就是一個可能的true path。
這個題目的目標就是在最短的時間內將這些真正會影響到電路輸出的關鍵路線，我們稱之為 critical path或是 true path找出來。
因為整個題目都在操作邏輯相關的知識，所以他其實是一個演算法的題目。

-
那我把我接下來要講的關於這方面的內容切成兩部分，一部份是coding 的部分，包含資料結構的設計、以及使用到C++的哪部分技術；另一部份是演算法設計的部分，
資料結構的設計主要是我負責的，我考慮了演算法可能會用到的 Function，並寫成各種 Api 供組員使用。
我主要設計了兩種 class, 一個是邏輯 gate, 一個是 gate 的 pin

CirGate 是存 Gate 的 class，Gate 總共有三種，NAND、NOR、NOT，因為這 三種 Gate 有雷同的功能， 
*  有 type 的不同:NAND、NOR、NOT 
*  三種 type 有各自的 controlling value 
* 有各自的邏輯意義 
* 前接兩個 FANIN 的 WIRE、後接一個 OUTPUT 的 WIRE 
* 對一個 path 來說，在 gate 上會有一個 offpin 於是我用繼承的方式寫了四個 clase。

gatepin 則是代表 gate 之間的 wire。
為什麼要這樣設計呢?
1. 有 wire 的情況下 邏輯清晰許多 不用所有東西都放在 gate 裡，link
gate 的時候很快速，因為之前有宣告過，讀圖的當下就可以找到這個 wire ,
不用額外再讀完之後後續每個 gate 都跑過一次來 link gate。
也較符合 verilog 的邏輯
沒什麼缺點，記憶體的容量跟放在 gate 裡的時候一樣，唯一可能是
construcot 會花比放在 gate 裡的多一點點時間，可是絕對會比前面那點所
說的後續要 linkgate 這件事少很多
2.不放的話，邏輯觀念不清，因沒有宣告 wire，讀完電路後還得每個
gate 跑過一次，非常花時間。

在每個 gate 和 wire 之間還有定義實現演算法需要的 function。
細節:
那資料結構是這樣設計。
再來說明建立這個資料結構的過程。
在資料結構的部分，都使用了自定義的hashMap 去儲存，由於 hashMap 的find函數時間複雜度為 O(1)。因為這些宣告過後的 gate 要透過 命名去取得他們的 pointer，於是我自定義了一個 hashMap希望他們搜尋速度降低。
因為是使用 Pointer 儲存 wire，所以不用再宣告完 gate 之後事後 link gate， 這樣也節省了許多時間。
在資料結構上，我做的 Optimzation 為這些。
