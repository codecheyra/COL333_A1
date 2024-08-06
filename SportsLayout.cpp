#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <chrono>
#include <ctime>

using namespace std;

#include "SportsLayout.h"

    SportsLayout::SportsLayout(string inputfilename)
    {
        
        readInInputFile(inputfilename);
        mapping= new int[z];
        best_mapping = new int[z];
        nei_best_mapping = new int[z];
        locations = new int[l];

        // sample_mapping = new int[z];

    }

    bool SportsLayout::check_output_format()
    {

        vector<bool> visited(l,false);
        for(int i=0;i<z;i++)
        {
            if((mapping[i]>=1 && mapping[i]<=l))
            {
                if(!visited[mapping[i]-1])
                visited[mapping[i]-1]=true;
                else
                {
                    cout<<"Repeated locations, check format\n";
                    return false;
                }
            }
            else
            {
                cout<<"Invalid location, check format\n";
                return false;
            }
        }

        return true;

    }


    long long SportsLayout::cost_fn(){


        long long cost=0;

        for(int i=0;i<z;i++)
        {
           for(int j=0;j<z;j++) 
           {
                cost+=(long long)N[i][j]*(long long)T[mapping[i]-1][mapping[j]-1];
           }
        }

        return cost;
    }

    void SportsLayout::readInInputFile(string inputfilename)
    {
            fstream ipfile;
	        ipfile.open(inputfilename, ios::in);
            if (!ipfile) {
                cout << "No such file\n";
                exit( 0 );
            }
            else {
                

                ipfile>> time;
                ipfile >> z;
                ipfile >> l;

                if(z>l)
                {
                    cout<<"Number of zones more than locations, check format of input file\n";
                    exit(0);
                }


            

            int **tempT;
            int **tempN;

          tempT = new int*[l];
         for (int i = 0; i < l; ++i)
            tempT[i] = new int[l];
        
        tempN = new int*[z];
        for (int i = 0; i < z; ++i)
            tempN[i] = new int[z];

        for(int i=0;i<z;i++)
        {
            for(int j=0;j<z;j++)
            ipfile>>tempN[i][j];
        }

        for(int i=0;i<l;i++)
        {
            for(int j=0;j<l;j++)
            ipfile>>tempT[i][j];
        }

        ipfile.close();

        T= tempT;
        N= tempN;
            }

    }

    void SportsLayout::write_to_file(string outputfilename){

         // Open the file for writing
    ofstream outputFile(outputfilename);

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }

    for(int i=0;i<z;i++)
    outputFile<<mapping[i]<<" ";

    // Close the file
    outputFile.close();

    cout << "Allocation written to the file successfully." << endl;

    }

    void SportsLayout::compute_allocation(string outputfilename)
    {
        //you can write your code here 
        //comment out following dummy code
        auto start_time = std::chrono::steady_clock::now();
        auto start = std::chrono::high_resolution_clock::now();
        auto end_time = start_time + std::chrono::seconds((long int)(time*60));
        vector<int> empty_locations;
        for (int i = 0; i < l;i++){
            locations[i] = i +1;
        }
        long long cost;
        long long best_cost;
        // empty locations vector
        int t1;
        int t2;
        for (int i = 0;i<l;i++){
            t1 = rand() % l;
            t2 = rand() % l;
            // swap s1 and s2 indices in locations array 
            int temp = locations[t1];
            locations[t1] = locations[t2];
            locations[t2] = temp;
        }
        for (int i = 0;i<z;i++){
            mapping[i] = locations[i];
        }
        empty_locations.clear();
        for (int i = z;i<l;i++){
            empty_locations.push_back(locations[i]);
        }
        cost = cost_fn();
        best_cost = cost;

        // intial mapping 
        // T time matrix 
        // N walk matrix 
        bool indicator = false;
        if (z < l){
            indicator = true;
        }
        bool counter = false;
        
        while (std::chrono::steady_clock::now() < end_time){
            // two cases
            // 1. swap two locations
            // 2. swap one location with a empty location
            bool found_best = false;
            long long new_cost;
            bool empty_best = false;
            int swap_index_1;
            int swap_index_2;
            int empty_index_1;
            int empty_index_2;
            
            long long initial_cost = cost;

            // swap locations
            for (int i = 0; i < z; i++){
                for (int j = i + 1; j < z; j++){
                    // check by swapping two locations
                    new_cost = initial_cost;
                    for ( int k =0 ;k<z;k++){
                        new_cost -= (N[i][k] + N[k][i]) *T[mapping[i]-1][mapping[k]-1];
                        new_cost -=  (N[k][j] + N[j][k]) * T[mapping[j]-1][mapping[k]-1];
                    }

                    int temp = mapping[i];
                    mapping[i] = mapping[j];
                    mapping[j] = temp;
                    for (int k = 0; k < z; k++){
                        new_cost += (N[i][k] + N[k][i]) * T[mapping[i]-1][mapping[k]-1];
                        new_cost += (N[k][j] + N[j][k]) * T[mapping[j]-1][mapping[k]-1];
                    }
                    // new_cost = cost_fn();
                    if (new_cost < cost){
                        cost = new_cost;
                        found_best = true;
                        swap_index_1 = i;
                        swap_index_2 = j;
                    }
                    // revert mapping
                    temp = mapping[i];
                    mapping[i] = mapping[j];
                    mapping[j] = temp;
                }
            }

            // swap one location with a empty location
            if (indicator){
                for (int i = 0; i < z; i++){
                    for (int j = 0; j < empty_locations.size(); j++){
                        new_cost = initial_cost;
                        for (int k = 0; k < z; k++){
                            new_cost -= (N[i][k] + N[k][i]) * T[mapping[i]-1][mapping[k]-1];
                        }
                        int temp = mapping[i];
                        mapping[i] = empty_locations[j];
                        empty_locations[j] = temp;
                        // new_cost = cost_fn();
                        for (int k = 0; k < z; k++){
                            new_cost += (N[i][k] + N[k][i]) * T[mapping[i]-1][mapping[k]-1];
                        }
                        if (new_cost < cost){
                            cost = new_cost;
                            // for (int i = 0; i < z; ++i) {
                            //     next_best_mapping[i] = mapping[i];
                            // }
                            found_best = true;
                            empty_best = true;
                            empty_index_1 = i;
                            empty_index_2 = j;
                        }
                        // revert mapping
                        temp = mapping[i];
                        mapping[i] = empty_locations[j];
                        empty_locations[j] = temp;
                        
                    }
                }

            }
            // check the best exists or not 
            if (found_best){
                if (empty_best){
                    int temp = mapping[empty_index_1];
                    mapping[empty_index_1] = empty_locations[empty_index_2];
                    empty_locations[empty_index_2] = temp;
                }else{
                    int temp = mapping[swap_index_1];
                    mapping[swap_index_1] = mapping[swap_index_2];
                    mapping[swap_index_2] = temp;
                }
                for (int i = 0; i < z; ++i) {
                    nei_best_mapping[i] = mapping[i];
                }

            }else{
                // random restart
                // take random z numbers from l numbers
                //assign the numbers to the mapping
                counter = true;
                if (cost < best_cost){
                    best_cost = cost;
                    write_to_file(outputfilename);
                    for (int i = 0; i < z; ++i) {
                        best_mapping[i] = mapping[i];
                    }
                }
                // random restart
                // cout << "bestcost : " << best_cost << endl;
                int s1;
                int s2;
                for (int i = 0;i<l;i++){
                    s1 = rand() % l;
                    s2 = rand() % l;
                    // swap s1 and s2 indices in locations array 
                    int temp = locations[s1];
                    locations[s1] = locations[s2];
                    locations[s2] = temp;
                }
                for (int i = 0;i<z;i++){
                    mapping[i] = locations[i];
                }
                empty_locations.clear();
                for (int i = z;i<l;i++){
                    empty_locations.push_back(locations[i]);
                }
                cost = cost_fn();
            }
            // write_to_file("output.txt");
            auto current_time = std::chrono::steady_clock::now();
            if (current_time >= end_time) {
                break; // Exit the loop if 2 minutes have passed
            }
        }
        // if (cost < best_cost){
        //             best_cost = cost;
        //             for (int i = 0; i < z; ++i) {
        //                 best_mapping[i] = mapping[i];
        //             }
        //         }
        cost = best_cost;
        if (counter){
            for (int i = 0; i < z; ++i) {
                mapping[i] = best_mapping[i];
            }
        }else{
            for (int i = 0; i < z; ++i) {
                mapping[i] = nei_best_mapping[i];
            }

        }
        
    }


