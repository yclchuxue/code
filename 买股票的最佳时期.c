int maxProfit(int* prices, int pricesSize){
    int minprice=0xFFFFFF;//用来记录历史最低的价格。
    int maxret =0;

    for(int i =0;i<pricesSize;i++){
        if(prices[i] < minprice){//判断今天是不是历史最低的价格。
            minprice = prices[i];
        }else{//如果不是历史最低的价格，判断如果今天卖掉的获利，是不是最大的？
            //printf("%d minprice :%d \n",i,minprice);
            maxret = (prices[i]-minprice)>maxret ? prices[i]-minprice:maxret;
           // printf("maxret :%d \n",maxret);
        }
    }
    return maxret;
}