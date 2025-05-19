//
// Created by Administrator on 25-5-9.
//

#ifndef OFFLINE_H
#define OFFLINE_H
#include "utilityfunction.h"
class A_query
{
public:
    A_query(int node_temp,double t_weight)
    {
        node=node_temp;
        weight=t_weight;
        l=1;
    }
    int node;
    double weight;
    int l;
};
bool cmp_smkrandom(const A_query &a,const A_query &b)
{
    return a.weight>b.weight;
}
S_class Offline(double B,double eps,const vector<int> & Sij, long long int &oracle_times)
{
    double default_p=0.5;

    default_random_engine engine(1);
    K=floor(B/min_cost);

    eps/=14.0;
    // cout<<"SmkRandomAcc ---------start--------- "<<endl;
    // cout<<"SmkRandomAcc & B: "<<B<<endl;

    S_class S;
    vector<int> Sset;

    vector<A_query> A;
    vector<A_query> A_augment;

    for(auto iter:Sij)
    {
        if(Groundset[iter].cost<=B)
        {
            oracle_times++;

            double value=f_u(iter);
            A_query temp(iter,value/Groundset[iter].cost);
            A.push_back(temp);

            A_query temp2(iter,value);
            A_augment.push_back(temp2);
        }
    }
    sort(A.begin(),A.end(),cmp_smkrandom);

    bernoulli_distribution u(default_p);
    while(!A.empty())
    {
        //argmax i
        int node_i=-1;
        double max_marginal_cost=-999999999;

        for(auto &u:A)
        {

            double old_wt=u.weight;

            if(max_marginal_cost>=old_wt) break;
            //update weight
            oracle_times++;
            u.weight=(S.marginal_gain(u.node)/Groundset[u.node].cost);
            if(u.weight>=(old_wt/(1+eps)))
            {
                node_i=u.node;
                max_marginal_cost=u.weight;
                break;
            }
            else
            {
                if(u.weight>max_marginal_cost)
                {
                    max_marginal_cost=u.weight;
                    node_i=u.node;
                }
                u.l++;
            }
            /*
            if((*u).l>((log((double)node_num/eps)/log(2.0))/eps))
            {
                u=A.erase(u);
                cout<<"happen!"<<endl;
            }
            sort(A.begin(),A.end(),cmp_smkrandom);
            */
        }
        if(node_i==-1)
        {
            cout<<"la ji"<<endl;
            break;
        }
        if(max_marginal_cost<0)
            break;

        double random=u(engine);
        if(random==1)
        {
//            S.Set.push_back(node_i);
//            S.S_cost+=node_cost[node_i];
//            S.S_revenue+=max_marginal_cost*node_cost[node_i];
//            S.selected[node_i]=1;
            S.add_element(max_marginal_cost*Groundset[node_i].cost,node_i);

            Sset.emplace_back(node_i);
        }
        for(auto e=A.begin();e!=A.end();)
        {
            if( (S.s_cost+Groundset[(*e).node].cost>B) || ((*e).node==node_i) || ((*e).weight<=0)
                || ((*e).l>(log(K/eps)/log(1+eps))) )
            {
                e=A.erase(e);
            }
            else
            {
                e++;
            }
        }
        sort(A.begin(),A.end(),cmp_smkrandom);
    }

    //test
    // cout<<"S:"<<endl;
    // cout<<"  revenue: "<<S.s_revenue<<" cost: "<<S.s_cost<<" size: "<<S.solution.size()<<endl;
    // for(const auto &p:S.solution)
    //     cout<<p<<" ";
    // cout<<endl;

    /*
    cout<<"S: "<<endl;
    cout<<"revenue: "<<S.S_revenue<<" cost: "<<S.S_cost<<endl;
    for(auto uu:S.Set)
        cout<<uu<<" ";
    cout<<endl;
*/

    //getmax for augment
    sort(A_augment.begin(),A_augment.end(),cmp_smkrandom);
    S_class S_star=S;
    //j=0,ie. argmax f(u)
    int i_star=-1;
    double i_star_value=-999999999;

    /*
    for(int iter=0;iter<node_num;iter++)
    {
        if(contrast_cost[iter]<=B)
        {
            //oracle_times++;
            double value=f_u(iter);
            if (value > i_star_value)
            {
                i_star_value = value;
                i_star = iter;
            }
        }
    }*/
    auto e_temp=A_augment.begin();
    i_star=(*e_temp).node;
    i_star_value=(*e_temp).weight;
    if(i_star_value>S_star.s_revenue)
    {
        S_star.replace_with_singleton(i_star_value,i_star);
    }

    //j>=1
    S_class Sj;
    for(int j=0;j<Sset.size();j++)
    {
        //build Sj
        int temp_e=Sset[j];//Sj-1 and temp_e = now Sj

        Sj.add_element_without_cal_revenue(temp_e);

        int uj=-1;
        double uj_value=-999999999;

        for(auto e=A_augment.begin();e!=A_augment.end();)
        {
            if( (Sj.s_cost+Groundset[(*e).node].cost>B) || ((*e).node==temp_e) || ((*e).weight<=0)
                || ((*e).l>(log(K/eps)/log(1+eps))) )
            {
                e=A_augment.erase(e);
            }
            else
            {
                e++;
            }
        }

        sort(A_augment.begin(),A_augment.end(),cmp_smkrandom);

        for(auto &u:A_augment)
        {
            double old_wt=u.weight;
            if(uj_value>=old_wt) break;

            //update weight
            oracle_times++;
            u.weight=Sj.marginal_gain(u.node);
            if(u.weight>=(old_wt/(1+eps)))
            {
                uj=u.node;
                uj_value=u.weight;
                break;
            }
            else
            {
                if(u.weight>uj_value)
                {
                    uj_value=u.weight;
                    uj=u.node;
                }
                u.l++;
            }
            /*
            if(u.l>((log((double)node_num/eps)/log(2.0))/eps))
            {
                u=A_augment.erase(u);
            }
            //*/
            //sort(A_augment.begin(),A_augment.end(),cmp_smkrandom);
        }
        if(uj==-1) continue;

        double f_Sj_and_uj=Sj.s_revenue+uj_value;

        if(f_Sj_and_uj>S_star.s_revenue)
        {
            S_star=Sj;
            S_star.add_element(uj_value,uj);
        }
    }

    // cout<<"S*:"<<endl;
    // cout<<"  revenue: "<<S_star.s_revenue<<" cost: "<<S_star.s_cost<<" size: "<<S_star.solution.size()<<endl;
    // for(const auto &p:S.solution)
    //     cout<<p<<" ";
    // cout<<endl;
    //
    // cout<<"oracle: "<<oracle_times<<endl;
    // cout<<"SmkRandomAcc ---------end--------- "<<endl;
    return S_star;
}
#endif //OFFLINE_H
