//
// Created by HP on 24-9-20.
//

#ifndef ONESTREAM_H
#define ONESTREAM_H
#include "utilityfunction.h"
//#include <ranges>
#include "Offline.h"
class Si {
    public:
    double sum_cost;
    vector<int> set;
    Si() {
        sum_cost = 0;
    };
    void add_element(const int& node) {
        set.emplace_back(node);
        sum_cost+=Groundset[node].cost;
    }
};
double theta=1.0/1;
Result OneStream(double B,double eps) {
    // cout<<"Theta:"<<theta<<endl;
    int h=ceil(log2(1.0/2.0/eps))+2;

    cout<<"OneStream ---------start--------- "<<endl;
    cout<<"B = "<<B<<endl;

    long long int query=0;
    long long int memory=0;
    int e_star=-1;
    double e_star_value=-999.0;

    // cout<<"h = "<<h<<endl;
    vector<Si> S_array;
    S_array.emplace_back();
    S_array.emplace_back();
    int i=1;
    int j=1;
    S_class T;
    for(int e=0;e<node_num;e++) {
        if(!budget_feasible_single(e,B))
            continue;

        //get e_star
        // double fe_value=f_u(e);
        // query++;
        // if(fe_value>e_star_value) {
        //     e_star_value=fe_value;
        //     e_star=e;
        // }


        //check marginal gain
        double marginal=T.marginal_gain(e);
        query++;
        if(marginal/Groundset[e].cost>=theta*T.s_revenue/B) {
            S_array[i].add_element(e);

            T.add_element(marginal,e);

            if(S_array[i].sum_cost>=B) {

                memory=max(memory,(long long int)T.solution.size());

                //delete sets
                if(i-j+1>=2*h) {
                    j=j+h;
                    //re-construct T
                    T.clear();
                    for(int it=j;it<=i;it++) {
                        T.add_set_element_without_cal_revenue(S_array[it].set);
                    }
                    T.s_revenue=T.f_S();
                }
                i++;
                S_array.emplace_back();
            }

        }
    }

    // cout<<"T cost: "<<T.s_cost<<endl;
    // cout<<"T node"<<endl;
    // for(const auto &p:T.solution)
    //     cout<<p<<" ";
    // cout<<endl;
    memory=max(memory,(long long int)T.solution.size());

    S_class Q;
    if(T.s_cost<=B) {
       // cout<<"Budget is not full !"<<endl;
        Q=T;
    }
    else {
        //we must access S_array to use vector since T store element using unorder_set
        // bool quit=false;
        vector<int> Sij_final;
        for(int it=i;it>=j;it--) {
            for (auto node = S_array[it].set.rbegin(); node != S_array[it].set.rend(); ++node) {
                Sij_final.emplace_back(*node);
            }
        }
        Q=Offline(B,eps,Sij_final,query);
    }
    if(e_star_value>Q.s_revenue)
        Q.replace_with_singleton(e_star);


    // cout<<"S*:"<<endl;
    // cout << "  revenue: " << Q.s_revenue << " cost: " <<Q.s_cost << " size: " << Q.solution.size() << endl;
    // cout<<" node"<<endl;
    // for(const auto &p:Q.solution)
    //     cout<<p<<" ";
    // cout<<endl;
    cout<<"Utility: "<<Q.f_S()<<endl;
    cout<<"Query: "<<query<<endl;
    cout<<"Memory: "<<memory<<endl;

    cout<<"OneStream ---------end--------- "<<endl<<endl;
    return Result(Q.s_revenue, Q.s_cost, Q.solution.size(), query,memory);
}
pair<S_class,double> OneStreamForMulti(double B,double eps,int h, long long int &query,long long int &memory) {
   // int h=log2(1.0/8.0/eps)+2;

    // cout<<"OneStreamForMulti ---------start--------- "<<endl;
    // cout<<"B = "<<B<<endl;

    int e_star=-1;
    double e_star_value=-999.0;

    // cout<<"h = "<<h<<endl;
    vector<Si> S_array;
    S_array.emplace_back();
    S_array.emplace_back();
    int i=1;
    int j=1;
    S_class T;
    for(int e=0;e<node_num;e++) {
        if(!budget_feasible_single(e,B))
            continue;

        //get e_star
        // double fe_value=f_u(e);
        // query++;
        // if(fe_value>e_star_value) {
        //     e_star_value=fe_value;
        //     e_star=e;
        // }


        //check marginal gain
        double marginal=T.marginal_gain(e);
        query++;
        if(marginal/Groundset[e].cost>=theta*T.s_revenue/B) {
            S_array[i].add_element(e);

            T.add_element(marginal,e);

            if(S_array[i].sum_cost>=B) {
                //delete sets
                if(i-j+1>=2*h) {
                    j=j+h;
                    //re-construct T
                    T.clear();
                    for(int it=j;it<=i;it++) {
                        T.add_set_element_without_cal_revenue(S_array[it].set);
                    }
                    T.s_revenue=T.f_S();
                }
                i++;
                S_array.emplace_back();
            }

        }
    }
    T.s_revenue=T.f_S();
    // cout<<"T revenue: "<<T.s_revenue<<endl;
    // cout<<"T cost: "<<T.s_cost<<endl;
    // cout<<"T node"<<endl;
    // for(const auto &p:T.solution)
    //     cout<<p<<" ";
    // cout<<endl;


    S_class Q;
    if(T.s_cost<=B) {
        // cout<<"Budget is not full !"<<endl;
        Q=T;
    }
    else {
        //we must access S_array for using vector as T store element using unorder_set
        // bool quit=false;
        for(int it=i;it>=j;it--) {
            // if(quit) break;
            for (auto node = S_array[it].set.rbegin(); node != S_array[it].set.rend(); ++node) {
                if(Q.s_cost+Groundset[*node].cost<=B) {
                    Q.add_element(0.0,*node);
                }
                /*******add more elements than designed without quit is not a bad thing*********/
                // else {
                //     Q.s_revenue=Q.f_S();
                //     quit=true;
                //     break;
                // }
            }
        }
        Q.s_revenue=Q.f_S();
    }
    if(e_star_value>Q.s_revenue)
        Q.replace_with_singleton(e_star);

    // cout<<"S*:"<<endl;
    // cout << "  revenue: " << Q.s_revenue << " cost: " <<Q.s_cost << " size: " << Q.solution.size() << endl;
    // cout<<" node"<<endl;
    // for(const auto &p:Q.solution)
    //     cout<<p<<" ";
    // cout<<endl;
    // cout<<"Real Revenue: "<<Q.f_S()<<endl;
    // cout<<"Query: "<<query<<endl;
    //
    // cout<<"OneStream ---------end--------- "<<endl<<endl;

    return make_pair(Q,T.s_revenue);
}
#endif //ONESTREAM_H

