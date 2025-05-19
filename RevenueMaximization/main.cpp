#include <stdlib.h>
#include "time.h"
#include "utilityfunction.h"
#include "OneStream.h"
#include "SmkStream.h"
#include "MultiStream.h"
#include "KnapStream.h"
int main(int argc,char *argv[]) {
    //random_weight();
    //random_cost();
    //renum();
    //cumulate_normalize_cost(0.2);

    time_t nowtime;
    struct tm* p;;
    time(&nowtime);
    p = localtime(&nowtime);

    read_data();
    string::size_type pos1, pos2,posend;
    pos1=node_cost_text.find_last_of("/");
    pos2=node_cost_text.rfind("/",pos1-1);

    posend=node_cost_text.find_last_not_of("/");
    string name1=node_cost_text.substr(pos2+1,pos1-pos2-1);
    string name2=node_cost_text.substr(pos1+1,posend);
    string result_name=name1+"_"+name2;
    //cout<<result_name<<endl;
    string outtext="./result/result_"+result_name+"_"+to_string(p->tm_mon+1)+"."+to_string(p->tm_mday)+"_"+to_string(p->tm_hour)+"_"+to_string(p->tm_min)+"_"+to_string(p->tm_sec)+".txt";


//    double B=atof(argv[1]);
 //   double B_ratio=atof(argv[1]);
    //double eps=atof(argv[2]);
    double eps=0.1;
    cout<<"eps: "<<eps<<endl;

    vector<Result> onestream_result;
    vector<Result> multistream_result;
    vector<Result> smkstream_result;
    vector<Result> knapstream_result;

    double B_start=50.0;
    double B_end=500.0;
    double B_step=45.0;
    for(double B=B_start;B<=B_end;B+=B_step) {
        int repeat_times=10;
        Result total_result_one(0, 0, 0);
        for (int i = 0; i < repeat_times; ++i) {
            Result current_result = OneStreamOffline(B, eps);
            total_result_one = total_result_one + current_result;
        }
        Result average_result_one = total_result_one / repeat_times;
        onestream_result.push_back(average_result_one);

        Result total_result_multi(0, 0, 0);
        for (int i = 0; i < repeat_times; ++i) {
            Result current_result = MultiStream(B, eps);
            total_result_multi = total_result_multi + current_result;
        }
        Result average_result_multi = total_result_multi / repeat_times;
        multistream_result.push_back(average_result_multi);

        //onestream_result.emplace_back(OneStreamOffline(B,eps));
       // multistream_result.emplace_back(MultiStream(B,eps));
          smkstream_result.emplace_back(SmkStream(B,eps));
          knapstream_result.emplace_back(KnapsackStreaming(B,eps));
    }


    ofstream out(outtext);
    out<<"eps: "<<eps<<endl;
    //out<<"real Budget: "<<endl;
    for(double B=B_start;B<=B_end;B+=B_step)
    {
        out<<B<<"\t";
    }
    out<<endl;

   out<<"OneStream "<<endl;
    out<<"utility: "<<endl;
    for(auto p:onestream_result)
    {
        out<<p.revenue<<"\t";
    }
    out<<endl;
    out<<"query: "<<endl;
    for(auto p:onestream_result)
    {
        out<<p.oracle<<"\t";
    }
    out<<endl;
    out<<"memory: "<<endl;
    for(auto p:onestream_result)
    {
        out<<p.memory<<"\t";
    }
    out<<endl;

    out<<"MultiStream "<<endl;
    out<<"utility: "<<endl;
    for(auto p:multistream_result)
    {
        out<<p.revenue<<"\t";
    }
    out<<endl;
    out<<"query: "<<endl;
    for(auto p:multistream_result)
    {
        out<<p.oracle<<"\t";
    }
    out<<endl;
    out<<"memory: "<<endl;
    for(auto p:multistream_result)
    {
        out<<p.memory<<"\t";
    }
    out<<endl;


    out<<"KnapsackStream "<<endl;
    out<<"revenue: "<<endl;
    for(auto p:knapstream_result)
    {
        out<<p.revenue<<"\t";
    }
    out<<endl;
    out<<"query: "<<endl;
    for(auto p:knapstream_result)
    {
        out<<p.oracle<<"\t";
    }
    out<<endl;
    out<<"memory: "<<endl;
    for(auto p:knapstream_result)
    {
        out<<p.memory<<"\t";
    }
    out<<endl;

    out<<"SmkStream "<<endl;
    out<<"revenue: "<<endl;
    for(auto p:smkstream_result)
    {
        out<<p.revenue<<"\t";
    }
    out<<endl;
    out<<"query: "<<endl;
    for(auto p:smkstream_result)
    {
        out<<p.oracle<<"\t";
    }
    out<<endl;
    out<<"memory: "<<endl;
    for(auto p:smkstream_result)
    {
        out<<p.memory<<"\t";
    }
    out<<endl;

    return 0;
}
