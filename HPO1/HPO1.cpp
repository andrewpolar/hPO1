//Concept: Andrew Polar and Mike Poluektov
//Developer Andrew Polar

//License
//In case if end user finds the way of making a profit by using this code and earns
//billions of US dollars and meet developer bagging change in the street near McDonalds,
//he or she is not in obligation to buy him a sandwich.

//Symmetricity
//In case developer became rich and famous by publishing this code and meet misfortunate
//end user who went bankrupt by using this code, he is also not in obligation to buy
//end user a sandwich.

//Publications:
//https://www.sciencedirect.com/science/article/abs/pii/S0016003220301149
//https://www.sciencedirect.com/science/article/abs/pii/S0952197620303742
//https://arxiv.org/abs/2305.08194

//This code shows hyper parameter optimization. No user definitions are necessary.

#include <iostream>
#include <thread>
#include <vector>
#include "KANAddendPL.h"
#include "Helper.h"
#include "Triangles.h"
#include "Circles.h"
#include "Formula.h"
#include "Data.h"

void Training(std::unique_ptr<std::unique_ptr<double[]>[]>& inputs, std::unique_ptr<double[]>& target,
    std::unique_ptr<std::unique_ptr<double[]>[]>& validationinputs, std::unique_ptr<double[]>& validationtarget,
    std::unique_ptr<std::vector<KANAddendPL>>& addends,
    int nRecords, int nValidationRecords, double mu, double targetMin,
    double targetMax, int innerMax, int outerMax, double& lastError) {

    double last4Errors[4];
    for (int k = 0; k < 4; ++k) last4Errors[k] = fabs(targetMax);
    double prev = 0.0;
    for (int i = 0; i < 4; ++i) prev += last4Errors[i];
    int epochCounter = 0;

    auto residuals = std::make_unique<double[]>(nRecords);
    double range = targetMax - targetMin;
    for (int i = 0; i < nRecords; ++i) {
        residuals[i] = range;
    }
    double delta = 0.01 * range;

    while (true) {

        //increment linear blocks for functions
        if (epochCounter > 0) {
            for (int j = 0; j < addends->size(); ++j) {
                if (addends->at(j).HowManyInner() < innerMax) {
                    addends->at(j).IncrementInner();
                }
            }
            for (int j = 0; j < addends->size(); ++j) {
                if (addends->at(j).HowManyOuter() < outerMax) {
                    addends->at(j).IncrementOuter();
                }
            }
        }

        //training
        for (int i = 0; i < nRecords; ++i) {
            double model = 0.0;
            for (int j = 0; j < addends->size(); ++j) {
                model += addends->at(j).ComputeUsingInput(inputs[i]);
            }
            double residual = target[i] - model;
            residuals[i] = fabs(residual);
            for (int j = 0; j < addends->size(); ++j) {
                addends->at(j).UpdateUsingMemory(residual * mu);
            }
        }
        //additional run through large error records
        for (int i = 0; i < nRecords; ++i) {
            if (residuals[i] < delta) continue;
            double model = 0.0;
            for (int j = 0; j < addends->size(); ++j) {
                model += addends->at(j).ComputeUsingInput(inputs[i]);
            }
            double residual = target[i] - model;
            residuals[i] = fabs(residual);
            for (int j = 0; j < addends->size(); ++j) {
                addends->at(j).UpdateUsingMemory(residual * mu);
            }
        }

        //validation
        double error = 0.0;
        double cnt = 0;
        for (int i = 0; i < nValidationRecords; ++i) {
            double model = 0.0;
            for (int j = 0; j < addends->size(); ++j) {
                model += addends->at(j).ComputeUsingInput(validationinputs[i]);
            }
            double residual = validationtarget[i] - model;
            error += residual * residual;
            ++cnt;
        }
        if (0 == cnt) cnt = 1;
        error /= cnt;
        error = sqrt(error);
        error /= range;
        printf("Training epoch %d, RRMSE for validation data %6.4f\r", epochCounter, error);
        lastError = error;

        //condition to exit
        int pos = epochCounter % 4;
        last4Errors[pos] = lastError;
        double next = 0.0;
        for (int i = 0; i < 4; ++i) next += last4Errors[i];
        if (fabs(prev - next) / (prev + next) < 0.001) break;
        prev = next;
        if (++epochCounter >= 500) break;
    }
    printf("\n");
}

int main()
{
    //constants
    const int innerMax = 8;
    const int outerMax = 16;
    const double mu = 0.01;

    srand((unsigned int)time(NULL));
    //std::unique_ptr<Data> data = std::make_unique<Circles>();
    //std::unique_ptr<Data> data = std::make_unique<Formula>();
    std::unique_ptr<Data> data = std::make_unique<Triangles>();
    bool status = data->Populate();
    if (false == status) {
        printf("Failed to open file");
        exit(0);
    }

    clock_t start_application = clock();
    auto helper = std::make_unique<Helper>();
    helper->Shuffle(data->inputs, data->target, data->nRecords, data->nFeatures);

    auto xmin = std::make_unique<double[]>(data->nFeatures);
    auto xmax = std::make_unique<double[]>(data->nFeatures);
    double targetMin;
    double targetMax;

    helper->FindMinMax(xmin, xmax, targetMin, targetMax, data->inputs, data->target,
        data->nRecords, data->nFeatures);

    int nModels;

    nModels = 1;
    auto addends1 = std::make_unique<std::vector<KANAddendPL>>();
    for (int i = 0; i < nModels; ++i) {
        addends1->push_back(KANAddendPL(xmin, xmax, targetMin / nModels, targetMax / nModels, 2, 2, data->nFeatures));
    }

    nModels = 2;
    auto addends2 = std::make_unique<std::vector<KANAddendPL>>();
    for (int i = 0; i < nModels; ++i) {
        addends2->push_back(KANAddendPL(xmin, xmax, targetMin / nModels, targetMax / nModels, 2, 2, data->nFeatures));
    }

    nModels = 3;
    auto addends3 = std::make_unique<std::vector<KANAddendPL>>();
    for (int i = 0; i < nModels; ++i) {
        addends3->push_back(KANAddendPL(xmin, xmax, targetMin / nModels, targetMax / nModels, 2, 2, data->nFeatures));
    }

    nModels = 5;
    auto addends5 = std::make_unique<std::vector<KANAddendPL>>();
    for (int i = 0; i < nModels; ++i) {
        addends5->push_back(KANAddendPL(xmin, xmax, targetMin / nModels, targetMax / nModels, 2, 2, data->nFeatures));
    }

    nModels = 8;
    auto addends8 = std::make_unique<std::vector<KANAddendPL>>();
    for (int i = 0; i < nModels; ++i) {
        addends8->push_back(KANAddendPL(xmin, xmax, targetMin / nModels, targetMax / nModels, 2, 2, data->nFeatures));
    }

    nModels = 13;
    auto addends13 = std::make_unique<std::vector<KANAddendPL>>();
    for (int i = 0; i < nModels; ++i) {
        addends13->push_back(KANAddendPL(xmin, xmax, targetMin / nModels, targetMax / nModels, 2, 2, data->nFeatures));
    }

    nModels = 21;
    auto addends21 = std::make_unique<std::vector<KANAddendPL>>();
    for (int i = 0; i < nModels; ++i) {
        addends21->push_back(KANAddendPL(xmin, xmax, targetMin / nModels, targetMax / nModels, 2, 2, data->nFeatures));
    }

    nModels = 34;
    auto addends34 = std::make_unique<std::vector<KANAddendPL>>();
    for (int i = 0; i < nModels; ++i) {
        addends34->push_back(KANAddendPL(xmin, xmax, targetMin / nModels, targetMax / nModels, 2, 2, data->nFeatures));
    }

    double lastError1 = 0.0;
    std::thread t1(Training, std::ref(data->inputs), std::ref(data->target), 
        std::ref(data->validationinputs), std::ref(data->validationtarget),
        std::ref(addends1),
        data->nRecords, data->nValidationRecords, mu, targetMin, targetMax, innerMax, outerMax, std::ref(lastError1));

    double lastError2 = 0.0;
    std::thread t2(Training, std::ref(data->inputs), std::ref(data->target),
        std::ref(data->validationinputs), std::ref(data->validationtarget),
        std::ref(addends2),
        data->nRecords, data->nValidationRecords, mu, targetMin, targetMax, innerMax, outerMax, std::ref(lastError2));

    double lastError3 = 0.0;
    std::thread t3(Training, std::ref(data->inputs), std::ref(data->target),
        std::ref(data->validationinputs), std::ref(data->validationtarget),
        std::ref(addends3),
        data->nRecords, data->nValidationRecords, mu, targetMin, targetMax, innerMax, outerMax, std::ref(lastError3));

    double lastError5 = 0.0;
    std::thread t5(Training, std::ref(data->inputs), std::ref(data->target),
        std::ref(data->validationinputs), std::ref(data->validationtarget),
        std::ref(addends5),
        data->nRecords, data->nValidationRecords, mu, targetMin, targetMax, innerMax, outerMax, std::ref(lastError5));

    double lastError8 = 0.0;
    std::thread t8(Training, std::ref(data->inputs), std::ref(data->target),
        std::ref(data->validationinputs), std::ref(data->validationtarget),
        std::ref(addends8),
        data->nRecords, data->nValidationRecords, mu, targetMin, targetMax, innerMax, outerMax, std::ref(lastError8));

    double lastError13 = 0.0;
    std::thread t13(Training, std::ref(data->inputs), std::ref(data->target),
        std::ref(data->validationinputs), std::ref(data->validationtarget),
        std::ref(addends13),
        data->nRecords, data->nValidationRecords, mu, targetMin, targetMax, innerMax, outerMax, std::ref(lastError13));

    double lastError21 = 0.0;
    std::thread t21(Training, std::ref(data->inputs), std::ref(data->target),
        std::ref(data->validationinputs), std::ref(data->validationtarget),
        std::ref(addends21),
        data->nRecords, data->nValidationRecords, mu, targetMin, targetMax, innerMax, outerMax, std::ref(lastError21));

    double lastError34 = 0.0;
    std::thread t34(Training, std::ref(data->inputs), std::ref(data->target),
        std::ref(data->validationinputs), std::ref(data->validationtarget),
        std::ref(addends34),
        data->nRecords, data->nValidationRecords, mu, targetMin, targetMax, innerMax, outerMax, std::ref(lastError34));

    t1.join();
    t2.join();
    t3.join();
    t5.join();
    t8.join();
    t13.join();
    t21.join();
    t34.join();

    clock_t end_PWL_training = clock();
    printf("Time for training %2.3f sec.\n\n", (double)(end_PWL_training - start_application) / CLOCKS_PER_SEC);

    printf("The following relative residual errors are obtained for different number of addends\n");
    printf("1 addend %6.4f\n", lastError1);
    printf("2 addends %6.4f\n", lastError2);
    printf("3 addends %6.4f\n", lastError3);
    printf("5 addends %6.4f\n", lastError5);
    printf("8 addends %6.4f\n", lastError8);
    printf("13 addends %6.4f\n", lastError13);
    printf("21 addends %6.4f\n", lastError21);
    printf("34 addends %6.4f\n", lastError34);

    //////////// Object copy test //////
    //auto addendsCopy = std::make_unique<std::vector<KANAddendPL>>();
    //for (int i = 0; i < addends->size(); ++i) {
    //    addendsCopy->push_back(KANAddendPL(addends->at(i)));
    //}
}
