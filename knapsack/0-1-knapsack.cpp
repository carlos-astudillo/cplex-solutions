#include <ilcplex/ilocplex.h>
#include <string>
ILOSTLBEGIN

int main(int argc, char **argv){
    
    IloEnv env;
    std::string filename = "0-1-knapsack.dat";
    try{
        // Read input file
        FILE *file_ptr = fopen(filename.c_str(), "r");
        
        // N = # of objects, C = Capacity of Knapsack
        int N = -1, C = -1;
        fscanf(file_ptr, "%d", &C);
        fscanf(file_ptr, "%d", &N);
        
        // declare model + variables
        IloModel model(env);
        IloIntVarArray x(env, N, 0, 1); // decision variable
        int w[N];
        int p[N];
        
        // Initialize variables
        for (int i = 0; i < N; ++i){
            fscanf(file_ptr, "%d %d", &w[i], &p[i]);
        }
        
        // objective + constraint    
        IloExpr objective(env);
        IloExpr constraint(env);
        for (int i = 0; i < N; ++i){
            objective += x[i] * p[i];
            constraint += x[i] * w[i];
        }
        model.add(IloMaximize(env, objective));
        model.add(constraint <= C);

        // solve the problem
        IloCplex cplex(model);
        if (!cplex.solve()){
            cout << "Failed to solve LP!" << endl;
            throw(-1);
        }

        IloNumArray x_vals(env);
        cout << "Solution Status = " << cplex.getStatus() << endl;
        cout << "Solution Value = " << cplex.getObjValue() << endl;
        cplex.getValues(x, x_vals);
        cout << "Objects = " << x_vals << endl; 

        fclose(file_ptr);
    }
    catch(IloException& e){
        cerr << "Concert exception caught: " << e << endl;
    }
    catch(...){
        cerr << "Unknown exception caught" << endl;
    }

    env.end();
    return 0;
}

