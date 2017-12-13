#include "experiment.h"



int main()
{

    while (1 == 1){
        printf("-----Choose experment-----\n");
        printf("1: Diffusion factor\n");
        printf("2: Thermal conductivity factor\n");
        printf("3: Viscosity factor\n");
        printf("4: Exit program\n");

        int state;
        scanf("%d", &state);

        if (state < 1 || state > 4){
            printf("Error: there is no such experiment\n\n");
            continue;
        }

        switch (state){
        case 1:
            Exper_Diffusion();
            break;
        case 2:
            Exper_Thermal_conductivity();
            break;
        case 3:
            Exper_Viscosity();
            break;
        case 4:
            return 0;
        }
    }


    return 0;
}










