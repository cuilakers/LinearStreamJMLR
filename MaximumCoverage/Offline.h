//
// Created by HP on 25-5-7.
//

#ifndef OFFLINE_H
#define OFFLINE_H
#include "utilityfunction.h"
#include "MultiStream.h"
double EstimatingOPT(const double & B,long long int &query) {
    S_class S;
    for(int e=0;e<node_num;e++) {
        if(!S.budget_feasible(e,B))
            continue;

        query++;
        double marginal=S.marginal_gain(e);
        if(marginal/Groundset[e].cost>=S.s_revenue)
            S.add_element(marginal,e);
    }
    // OPT/2<f(S)<4 OPT
    // f(S)/4 < OPT <8 f(S)/4
    return S.s_revenue/4.0;
}
// int non_monotone=false;
S_class Offline(double B,double eps,const vector<int> & Sij, long long int &query)
{
    //eps=0.2;
    double OPT_guess=EstimatingOPT(B,query);

    cout<<"Offline ---------start--------- "<<endl;
    cout<<"B = "<<B<<endl;
    int solution_num=1;
    // if(non_monotone) {
    //     solution_num=2;
    // }
    // else {
    //     solution_num=1;
    // }
    S_class S_best;
    vector<S_pair> S_array;
    double max_gamma=0.0;//max gamma in S_array, help to update S_array
    int min_gamma_index_in_C=-1;//-1 means S_array is empty, else means min gamma index which is still available now
    int max_gamma_index_in_C=-1;
   // int onepass_i=0;
    //double K;

    //int sum_gamma_num=0;
    // double LB_ave=0.0;
    // double fu_ave=0.0;
    for(auto u:Sij)
    {
        //generate C
        if(!budget_feasible_single(u,B))
            continue;

        double left_temp=(1.0 - eps)* S_best.s_revenue / ( 2.0*B);
        double right_temp=4.0*OPT_guess/ (eps * B );

        query++;
        double right_temp_element=f_u(u)/Groundset[u].cost;
        right_temp=min(right_temp_element,right_temp);

        // LB_ave+=S_best.s_revenue;
        // fu_ave+=right_temp;

        if(left_temp>right_temp) continue;

        double left=ceil(log(left_temp)/log(1.0/(1.0-eps) ));
        double right=floor(log(right_temp)/log(1.0/(1.0-eps)));

        //cout<<"gamma_num:"<<right-left<<endl;
        //sum_gamma_num+=(right-left);

        //vector<double> C;
        /*******fix bug*******/
        left=max(left,1.0);
        right=max(right,1.0);
        /*******fix bug*******/
        if(min_gamma_index_in_C==-1)//first visit S_array
        {
            for(int t=left;t<=right;t++)
            {
                double gamma_temp=pow(1.0/(1.0-eps),t);
                S_array.push_back(S_pair(gamma_temp));
            }
            min_gamma_index_in_C=0;//the index of min gamma which is still available now
            max_gamma=pow(1.0/(1.0-eps),right);//now max gamma
            max_gamma_index_in_C=S_array.size()-1;//max gamma index in C now
        }
        else//not first visit
        {
            double now_min_gamma_in_C=pow(1.0/(1.0-eps),left);//min gamma in C now
            double now_max_gamma_in_C=pow(1.0/(1.0-eps),right);//max gamma in C now
            if(now_min_gamma_in_C>max_gamma)//all old S_arrary should be removed
            {

                min_gamma_index_in_C=S_array.size();//the index of min gamma which is still available now
                for(int t=left;t<=right;t++)//then add new S gamma pair
                {
                    double gamma_temp=pow(1.0/(1.0-eps),t);
                    S_array.push_back(S_pair(gamma_temp));
                }
                max_gamma_index_in_C=S_array.size()-1;//max gamma index in C now
                max_gamma = S_array[S_array.size() - 1].gamma;//the last element always is the max_gamma anyway
            }
            else//else find where is the min gamma index now, is equivalent to remove all S gamma < left
            {
                bool need_update= true;//judge S_array shouble be updated or not
                for (int iter = min_gamma_index_in_C; iter < S_array.size(); iter++)
                {
                    if (S_array[iter].gamma < now_min_gamma_in_C)
                        min_gamma_index_in_C++;
                    if (S_array[iter].gamma >= now_max_gamma_in_C)
                    {
                        max_gamma_index_in_C=iter;
                        need_update=false;
                        break;
                    }
                }
                if(need_update)
                {
                    //finally, go through all gamma now, put new S gamma pair if needed
                    for (int t = left; t <= right; t++)
                    {
                        double gamma_temp = pow(1.0/(1.0-eps), t);
                        if (gamma_temp > max_gamma) {
                            S_array.push_back(S_pair(gamma_temp));
                        }
                    }
                    max_gamma = S_array[S_array.size() - 1].gamma;//the last element always is the max_gamma anyway
                    max_gamma_index_in_C=S_array.size() - 1;
                }
            }
        }
        //foreach gamma\in C
        for(int iter=min_gamma_index_in_C;iter<=max_gamma_index_in_C;iter++)
        {
            int max_solution=0;
            // double max_marginal=-999.0;
            // for (int x = 0; x < solution_num; ++x) {
            //     double temp_marginal=S_array[iter].spair[x].marginal_gain(u);
            //     if(temp_marginal>=max_marginal)
            //     {
            //         max_marginal=temp_marginal;
            //         max_solution=x;
            //     }
            // }
            if(S_array[iter].spair[max_solution].budget_feasible(u,B)) {
                double max_marginal=S_array[iter].spair[max_solution].marginal_gain(u);
                query++;
                // if(max_marginal/Groundset[u].cost>=(S_array[iter].gamma-S_array[iter].spair[max_solution].s_revenue)/(B-S_array[iter].spair[max_solution].s_cost))

                if(max_marginal/Groundset[u].cost>=S_array[iter].gamma)
                {
                    S_array[iter].spair[max_solution].add_element(max_marginal,u);
                }
            }
            if(S_array[iter].spair[max_solution].s_revenue > S_best.s_revenue)
            {
                S_best=S_array[iter].spair[max_solution];
            }
        }
    }
    cout<<"S_array.size(): "<<S_array.size()<<endl;
    //second pass
    for(int e=0;e<node_num;e++)
    {
        //foreach S^gamma
        for(int iter=min_gamma_index_in_C;iter<S_array.size();iter++)
        {
            for(auto &S_g:S_array[iter].spair)
            {
                //c(S \cup e)>B
                if(!S_g.budget_feasible(e,B))
                    continue;
                //judge if e\in S^gamma
                if(S_g.solution.count(e))
                    continue;

                query++;
                double marginal=S_g.marginal_gain(e);
                double f_S_g_and_e=marginal+S_g.s_revenue;
                if(f_S_g_and_e>S_best.s_revenue)
                {
                    S_best=S_g;
                    S_best.add_element(marginal,e);
                }
            }
        }
    }
    //foreach S^gamma to boost the best solution
    for(int iter=min_gamma_index_in_C;iter<S_array.size();iter++)
    {
        for(auto &S_g:S_array[iter].spair)
        {
            for(auto &e:S_g.solution) {
                if(!S_best.budget_feasible(e,B))
                    continue;
                if(S_best.solution.count(e))
                    continue;
                S_best.add_element_without_cal_revenue(e);
            }
        }
    }
    S_best.s_revenue=S_best.f_S();

    cout<<"S*:"<<endl;
    cout << "  revenue: " << S_best.s_revenue << " cost: " << S_best.s_cost << " size: " << S_best.solution.size() << endl;
    cout<<" node"<<endl;
    for(const auto &p:S_best.solution)
        cout<<p<<" ";
    cout<<endl;
    cout<<"Real Revenue: "<<S_best.f_S()<<endl;
    cout<<"Query: "<<query<<endl;

    cout<<"Offline ---------end--------- "<<endl<<endl;

    return S_best;
}


#endif //OFFLINE_H
