int maxProfit(int* prices, int pricesSize){
    int ans = 0;

    if(pricesSize>=2){                          //交易日不满两天直接返回0
        int max = prices[pricesSize-1],         //将max初始化为最后一天的值
        int profit;
        for(int i = pricesSize-2;i>=0;i--){     //从最后一天开始向第一天遍历
            profit = max-prices[i];              //计算当前的profit
            if(profit<=0)
                max = prices[i];
            else if(profit>ans)
                ans = profit;
        }
    }

    return ans;
}