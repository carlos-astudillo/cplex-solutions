#include <ilcplex/ilocplex.h>
#include <string>
ILOSTLBEGIN

typedef IloArray<IloIntVarArray> IloIntVarArray2;

int main(int argc, char **argv){
    
    if (argc < 2){
        cout << "usage: ./mmkp <input-filename>" << endl;
    }
    
    IloEnv env;
    try{
        std::string filename(argv[1]);
        // Read input file
        FILE *file_ptr = fopen(filename.c_str(), "r");
        
        // Declare variable and read data
        int kNumGroup = -1, kObjectPerGroup = -1, kResourcePerObject = -1;
        fscanf(file_ptr, "%d %d %d", &kNumGroup, &kObjectPerGroup, &kResourcePerObject);
        int resourceCaps[kResourcePerObject];
        double profitPerObject[kNumGroup][kObjectPerGroup];
        int resourceRequirements[kNumGroup][kObjectPerGroup][kResourcePerObject];

        for (int i = 0; i < kResourcePerObject; ++i){
            fscanf(file_ptr, "%d", &resourceCaps[i]);
        }
        int groupId;
        for (int i = 0; i < kNumGroup; ++i){
            fscanf(file_ptr, "%d", &groupId);
            for (int j = 0; j < kObjectPerGroup; ++j){
                fscanf(file_ptr, "%lf", &profitPerObject[i][j]);
                for (int k = 0; k < kResourcePerObject; ++k){
                    fscanf(file_ptr, "%d", &resourceRequirements[i][j][k]);
                }
            }
        }

        /*
        for (int i = 0; i < kNumGroup; ++i){
            cout << "Group " << i << endl;
            for (int j = 0; j < kObjectPerGroup; ++j){
                cout << profitPerObject[i][j] << " ";
                for (int k = 0; k < kResourcePerObject; ++k){
                    cout << resourceRequirements[i][j][k] << " ";
                }
                cout << endl;
            }
        }
        */

        // declare model + variables
        IloModel model(env);
        
        // Initialize variables
        IloIntVarArray2 x(env, kNumGroup);
        for (int i = 0; i < kNumGroup; ++i){
            x[i] = IloIntVarArray(env, kObjectPerGroup, 0, 1);
        }

        // objective + constraint    
        IloExpr objective(env);
        IloExprArray constraints(env, kResourcePerObject);
        IloExprArray oneObjectPerGroup(env, kNumGroup);
        for (int k = 0; k < kResourcePerObject; ++k){
            constraints[k] = IloExpr(env);
        }
        for (int i = 0; i < kNumGroup; ++i){
            oneObjectPerGroup[i] = IloExpr(env);
        }
        //construct the objective function
        for (int i = 0; i < kNumGroup; ++i){
            for (int j = 0; j < kObjectPerGroup; ++j){
                objective += x[i][j] * profitPerObject[i][j];
            }
        }                
        
        //build the constraints
        for (int k = 0; k < kResourcePerObject; ++k){
            for (int i = 0; i < kNumGroup; ++i){
                for (int j = 0; j < kObjectPerGroup; ++j){
                    constraints[k] += x[i][j] * resourceRequirements[i][j][k];
                }
            }
        }

        for (int i = 0; i < kNumGroup; ++i){
            for (int j = 0; j < kObjectPerGroup; ++j){
                oneObjectPerGroup[i] += x[i][j];
            }
        }

        model.add(IloMaximize(env, objective));
        for (int k = 0; k < kResourcePerObject; ++k){
            model.add(constraints[k] <= resourceCaps[k]);
        }
        for (int i = 0; i < kNumGroup; ++i){
            model.add(oneObjectPerGroup[i] == 1);
        }

        // solve the problem
        IloCplex cplex(model);
        if (!cplex.solve()){
            cout << "Failed to solve LP!" << endl;
            throw(-1);
        }

        cout << "Solution Status = " << cplex.getStatus() << endl;
        cout << "Solution Value = " << cplex.getObjValue() << endl;
        
        for (int i = 0; i < kNumGroup; ++i){
            IloNumArray x_val(env, kObjectPerGroup);
            cplex.getValues(x[i], x_val);
            cout << "Group: " << (i+1) << " "; 
            for (int j = 0; j < kObjectPerGroup; ++j){
                if (x_val[j] == 1){
                    cout << "Object: " << (j+1) << endl;
                }
            }
        }

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

