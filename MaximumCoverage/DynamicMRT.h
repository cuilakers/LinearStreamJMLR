//
// Created by HP on 24-9-21.
//

#ifndef DYNAMICMRT_H
#define DYNAMICMRT_H
#include "SmkStream.h"
/*Result DynamicMRT(double B,double eps)
{
    long long int query=0;
    long long int memory=0;
    cout<<"DynamicMRT ---------start--------- "<<endl;
    cout<<"B = "<<B<<endl;
    double alpha=2.0/3.0;
    int solution_num=1;
    //eps/=8.0;

    query=0;
    S_class S_best;
    vector<S_pair> S_array;
    double max_gamma=0.0;//max gamma in S_array, help to update S_array
    int min_gamma_index_in_C=-1;//-1 means S_array is empty, else means min gamma index which is still available now
    int max_gamma_index_in_C=-1;
   // int onepass_i=0;
    //double K;

    //int sum_gamma_num=0;
    double LB_ave=0.0;
    double fu_ave=0.0;
    double m=0.0;
    for(int u=0;u<node_num;u++)
    {
        //generate C
        if(!budget_feasible_single(u,B))
            continue;

        //onepass_i++;
       // K=(pi*pi*(1+3*eps)*(double)onepass_i*(double)onepass_i)/(6.0*eps);

        query++;

        double fu_value=f_u(u);
        // if(fu_value>S_best.s_revenue)
        // {
        //     S_best.replace_with_singleton(fu_value,u);
        // }
        if(fu_value<=0) continue;
        m=max(fu_value/Groundset[u].cost,m);

        double left_temp= m/(1+3*eps);
        double right_temp=2*B*m;

        LB_ave+=S_best.s_revenue;
        fu_ave+=right_temp;

        if(left_temp>right_temp) continue;

        double left=ceil(log(left_temp)/log(1+3*eps));
        double right=floor(log(right_temp)/log(1+3*eps));

        //cout<<"gamma_num:"<<right-left<<endl;
        //sum_gamma_num+=(right-left);

        //vector<double> C;
        /*******fix bug******#1#
        left=max(left,1.0);
        right=max(right,1.0);
        /*******fix bug******#1#
        if(min_gamma_index_in_C==-1)//first visit S_array
        {
            for(int t=left;t<=right;t++)
            {
                double gamma_temp=pow(1+3*eps,t);
                S_array.push_back(S_pair(gamma_temp));
            }
            min_gamma_index_in_C=0;//the index of min gamma which is still available now
            max_gamma=pow(1+3*eps,right);//now max gamma
            max_gamma_index_in_C=S_array.size()-1;//max gamma index in C now
        }
        else//not first visit
        {
            double now_min_gamma_in_C=pow(1+3*eps,left);//min gamma in C now
            double now_max_gamma_in_C=pow(1+3*eps,right);//max gamma in C now
            if(now_min_gamma_in_C>max_gamma)//all old S_arrary shouble be removed
            {
                min_gamma_index_in_C=S_array.size();//the index of min gamma which is still available now
                for(int t=left;t<=right;t++)//then add new S gamma pair
                {
                    double gamma_temp=pow(1+3*eps,t);
                    S_array.push_back(S_pair(gamma_temp));
                }
                max_gamma_index_in_C=S_array.size()-1;//max gamma index in C now
                max_gamma = S_array[S_array.size() - 1].gamma;//the last element always is the max_gamma anyway
            }
            else//else find where is the min gamma index now, is equivalent to remove all S gamma < left
            {
                bool need_update= true;//judge S_array should be updated or not
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
                        double gamma_temp = pow(1 + 3*eps, t);
                        if (gamma_temp > max_gamma)
                            S_array.push_back(S_pair(gamma_temp));
                    }
                    max_gamma = S_array[S_array.size() - 1].gamma;//the last element always is the max_gamma anyway
                    max_gamma_index_in_C=S_array.size() - 1;
                }
            }
        }
        // cout<<"S_array.size(): "<<max_gamma_index_in_C-min_gamma_index_in_C<<endl;
        //foreach gamma\in C
        for(int iter=min_gamma_index_in_C;iter<=max_gamma_index_in_C;iter++)
        {
            int max_solution=0;
            double max_marginal=S_array[iter].spair[0].marginal_gain(u);

            query++;
            if(Groundset[u].cost>=B/2.0&&f_u(u)/Groundset[u].cost>=2*S_array[iter].gamma/(3*B)) {
                S_array[iter].spair[0].replace_with_singleton(u);
                continue;
            }

            query++;
            // for (int x = 0; x < solution_num; ++x) {
            //     double temp_marginal=S_array[iter].spair[x].marginal_gain(u);
            //     query++;
            //     if(temp_marginal>=max_marginal)
            //     {
            //         max_marginal=temp_marginal;
            //         max_solution=x;
            //     }
            // }

            if((max_marginal/Groundset[u].cost>=2*S_array[iter].gamma/(3*B))
                    &&(S_array[iter].spair[max_solution].budget_feasible(u,B))) {
                S_array[iter].spair[max_solution].add_element(max_marginal,u);
            }

            if(S_array[iter].spair[max_solution].s_revenue > S_best.s_revenue)
            {
                S_best=S_array[iter].spair[max_solution];
            }


        }
    }
    cout<<"S_array.size(): "<<S_array.size()-min_gamma_index_in_C<<endl;

    for(int iter=min_gamma_index_in_C;iter<S_array.size();iter++) {
        for(auto &S_g:S_array[iter].spair)
        {
            memory+=S_g.solution.size();
        }
    }


    cout<<"S*:"<<endl;
    cout << "  revenue: " << S_best.s_revenue << " cost: " << S_best.s_cost << " size: " << S_best.solution.size() << endl;
    cout<<" node"<<endl;
    for(const auto &p:S_best.solution)
        cout<<p<<" ";
    cout<<endl;
    cout<<"Real Revenue: "<<S_best.f_S()<<endl;
    cout<<"Query: "<<query<<endl;
    cout<<"Memory: "<<memory<<endl;
    // int violation=0;
    // cout<<"size in each group: "<<endl;
    // for(int j=0;j<group_num;j++)
    // {
    //     cout<<S_best.group_exist[j]<<" ";
    // }
    // cout<<endl;
    // cout<<" number of fairness violation: "<<endl;
    // int vio_upper=0;
    // int vio_lower=0;
    // for(int j=0;j<S_best.group_exist.size();j++)
    // {
    //     int err=max(S_best.group_exist[j]-group_upper[j],group_lower[j]-S_best.group_exist[j]);
    //     err=max(err,0);
    //     violation+=err;
    //     cout<<err<<" ";
    //
    //     vio_upper+=max(S_best.group_exist[j]-group_upper[j],0);
    //     vio_lower+=max(group_lower[j]-S_best.group_exist[j],0);
    // }
    // cout<<endl;
    // cout<<"sum violation: "<<violation<<endl;
    // cout<<"upper bound violattion: "<<vio_upper<<endl;
    // cout<<"lower bound violattion: "<<vio_lower<<endl;

    cout<<"DynamicMRT ---------end--------- "<<endl<<endl;
    return Result(S_best.s_revenue, S_best.s_cost, S_best.solution.size(), query,memory);
}*/
Result DynamicMRT(double B,double eps)
{
    long long int query=0;
    long long int memory=0;
    cout<<"DynamicMRT ---------start--------- "<<endl;
    cout<<"B = "<<B<<endl;
    double alpha=2.0/3.0;
    int solution_num=1;
    //eps/=8.0;

    query=0;
    S_class S_best;
    vector<S_pair> S_array;
    double max_gamma=0.0;//max gamma in S_array, help to update S_array
    int min_gamma_index_in_C=-1;//-1 means S_array is empty, else means min gamma index which is still available now
    int max_gamma_index_in_C=-1;
   // int onepass_i=0;
    //double K;

    //int sum_gamma_num=0;
    double LB_ave=0.0;
    double fu_ave=0.0;
    double m=0.0;
    for(int u=0;u<node_num;u++)
    {
        //generate C
        if(!budget_feasible_single(u,B))
            continue;

        //onepass_i++;
       // K=(pi*pi*(1+eps)*(double)onepass_i*(double)onepass_i)/(6.0*eps);

        query++;

        double fu_value=f_u(u);
        // if(fu_value>S_best.s_revenue)
        // {
        //     S_best.replace_with_singleton(fu_value,u);
        // }
        if(fu_value<=0) continue;
        m=max(fu_value,m);

        double left_temp= m;
        double right_temp=B*m/alpha;

        LB_ave+=S_best.s_revenue;
        fu_ave+=right_temp;

        if(left_temp>right_temp) continue;

        double left=ceil(log(left_temp)/log(1+eps));
        double right=floor(log(right_temp)/log(1+eps));

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
                double gamma_temp=pow(1+eps,t);
                S_array.push_back(S_pair(gamma_temp));
            }
            min_gamma_index_in_C=0;//the index of min gamma which is still available now
            max_gamma=pow(1+eps,right);//now max gamma
            max_gamma_index_in_C=S_array.size()-1;//max gamma index in C now
        }
        else//not first visit
        {
            double now_min_gamma_in_C=pow(1+eps,left);//min gamma in C now
            double now_max_gamma_in_C=pow(1+eps,right);//max gamma in C now
            if(now_min_gamma_in_C>max_gamma)//all old S_arrary shouble be removed
            {
                min_gamma_index_in_C=S_array.size();//the index of min gamma which is still available now
                for(int t=left;t<=right;t++)//then add new S gamma pair
                {
                    double gamma_temp=pow(1+eps,t);
                    S_array.push_back(S_pair(gamma_temp));
                }
                max_gamma_index_in_C=S_array.size()-1;//max gamma index in C now
                max_gamma = S_array[S_array.size() - 1].gamma;//the last element always is the max_gamma anyway
            }
            else//else find where is the min gamma index now, is equivalent to remove all S gamma < left
            {
                bool need_update= true;//judge S_array should be updated or not
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
                        double gamma_temp = pow(1 + eps, t);
                        if (gamma_temp > max_gamma)
                            S_array.push_back(S_pair(gamma_temp));
                    }
                    max_gamma = S_array[S_array.size() - 1].gamma;//the last element always is the max_gamma anyway
                    max_gamma_index_in_C=S_array.size() - 1;
                }
            }
        }
        // cout<<"S_array.size(): "<<max_gamma_index_in_C-min_gamma_index_in_C<<endl;
        //foreach gamma\in C
        for(int iter=min_gamma_index_in_C;iter<=max_gamma_index_in_C;iter++)
        {
            int max_solution=-1;
            double max_marginal=-999.0;
            for (int x = 0; x < solution_num; ++x) {
                double temp_marginal=S_array[iter].spair[x].marginal_gain(u);
                query++;
                if(temp_marginal>=max_marginal)
                {
                    max_marginal=temp_marginal;
                    max_solution=x;
                }
            }
            if(max_solution!=-1)
            {
                if((max_marginal/Groundset[u].cost>=(alpha*S_array[iter].gamma-S_array[iter].spair[max_solution].s_revenue)/(B-S_array[iter].spair[max_solution].s_cost))
                    &&(S_array[iter].spair[max_solution].budget_feasible(u,B)))
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
    // cout<<"S_array.size(): "<<S_array.size()-min_gamma_index_in_C<<endl;

    for(int iter=min_gamma_index_in_C;iter<S_array.size();iter++) {
        for(auto &S_g:S_array[iter].spair)
        {
            memory+=S_g.solution.size();
        }
    }


    // cout<<"S*:"<<endl;
    // cout << "  revenue: " << S_best.s_revenue << " cost: " << S_best.s_cost << " size: " << S_best.solution.size() << endl;
    // cout<<" node"<<endl;
    // for(const auto &p:S_best.solution)
    //     cout<<p<<" ";
    // cout<<endl;
    cout<<"Utility: "<<S_best.f_S()<<endl;
    cout<<"Query: "<<query<<endl;
    cout<<"Memory: "<<memory<<endl;
    // int violation=0;
    // cout<<"size in each group: "<<endl;
    // for(int j=0;j<group_num;j++)
    // {
    //     cout<<S_best.group_exist[j]<<" ";
    // }
    // cout<<endl;
    // cout<<" number of fairness violation: "<<endl;
    // int vio_upper=0;
    // int vio_lower=0;
    // for(int j=0;j<S_best.group_exist.size();j++)
    // {
    //     int err=max(S_best.group_exist[j]-group_upper[j],group_lower[j]-S_best.group_exist[j]);
    //     err=max(err,0);
    //     violation+=err;
    //     cout<<err<<" ";
    //
    //     vio_upper+=max(S_best.group_exist[j]-group_upper[j],0);
    //     vio_lower+=max(group_lower[j]-S_best.group_exist[j],0);
    // }
    // cout<<endl;
    // cout<<"sum violation: "<<violation<<endl;
    // cout<<"upper bound violattion: "<<vio_upper<<endl;
    // cout<<"lower bound violattion: "<<vio_lower<<endl;

    cout<<"DynamicMRT ---------end--------- "<<endl<<endl;
    return Result(S_best.s_revenue, S_best.s_cost, S_best.solution.size(), query,memory);
}
#endif //DYNAMICMRT_H
