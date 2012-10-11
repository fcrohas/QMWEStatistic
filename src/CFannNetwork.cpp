#include "CFannNetwork.h"
#include <QtGui>

extern "C" {
    int FANN_API test_callback(struct fann *ann, struct fann_train_data *train,
            unsigned int max_epochs, unsigned int epochs_between_reports,
            float desired_error, unsigned int epochs)
    {
            printf("Epochs     %8d. MSE: %.5f. Desired-MSE: %.5f\n", epochs, fann_get_MSE(ann), desired_error);
            return 0;
    }

}


CFannNetwork::CFannNetwork(QObject *parent) :
    QObject(parent)
,num_input(90)
,num_output(26)
,num_layers(3)
,num_neurons_hidden(100)
,desired_error(0)
,max_epochs(1000)
,epochs_between_reports(10)
,hidden_activation(FANN_SIGMOID_SYMMETRIC)
,output_activation(FANN_SIGMOID_SYMMETRIC)
,stop_function(FANN_STOPFUNC_BIT)
,train_algorithm(FANN_TRAIN_RPROP)
{
}


CFannNetwork::~CFannNetwork() {
    if (data != NULL)
        fann_destroy_train(data);
    if (ann !=NULL)
        fann_destroy(ann);
}

void CFannNetwork::initializeNetwork()
{
    // Build Network with parameters
    ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
}

float CFannNetwork::buildNetwork(void)
{
    fann_set_activation_steepness_hidden(ann, 1);
    fann_set_activation_steepness_output(ann, 1);

    fann_set_activation_function_hidden(ann, (fann_activationfunc_enum)hidden_activation);
    fann_set_activation_function_output(ann, (fann_activationfunc_enum)output_activation);

    fann_set_train_stop_function(ann, (fann_stopfunc_enum)stop_function);
    fann_set_bit_fail_limit(ann, 0.01f);

    fann_set_training_algorithm(ann, (fann_train_enum)train_algorithm);

    fann_init_weights(ann, data);

    fann_train_on_data(ann, data, max_epochs, epochs_between_reports, desired_error);

    return fann_test_data(ann, data);
}

QList<CFannNetwork::record> CFannNetwork::testNetwork(void)
{
    unsigned int i;

    QList<record> result;

    if((!ann) && (!data))
    {
            return result;
    }

    for(i = 0; i < fann_length_train_data(data); i++)
    {
            fann_reset_MSE(ann);
            calc_out = fann_test(ann, data->input[i], data->output[i]);
            record trainResult;
            trainResult.line = i;
            //qDebug() << "Line " << i << "\n";
            trainResult.input = new float[num_input];
            trainResult.input = data->input[i];
            trainResult.want = new float[num_output];
            trainResult.output = new float[num_output];
            trainResult.difference = new float[num_output];

            for (unsigned int j=0; j < num_output;j++) {
                trainResult.want[j] = data->output[i][j];
                trainResult.output[j] = calc_out[j];
                trainResult.difference[j] = (float) fann_abs(calc_out[j] - data->output[i][j]);
            }
            result.append(trainResult);
    }
    return result;
}

void CFannNetwork::setTrainFile(QString file) {
    data = fann_read_train_from_file(file.toAscii().data());
}

void CFannNetwork::setTestFile(QString file) {
    data = fann_read_train_from_file(file.toAscii().data());
}

void CFannNetwork::loadNetwork(QString file) {
    ann = fann_create_from_file(file.toAscii().data());
}

void CFannNetwork::saveNetwork(QString file) {
    fann_save(ann, file.toAscii().data());
}
