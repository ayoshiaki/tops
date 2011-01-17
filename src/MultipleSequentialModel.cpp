/*
 *       MultipleSequentialModel.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU  General Public License as published by
 *       the Free Software Foundation; either version 3 of the License, or
 *       (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA.
 */

#include "MultipleSequentialModel.hpp"
#include "TrainMultinomialDistribution.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "Symbol.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>

namespace tops {

    void restore_submodel(std::string & model_name, std::map<std::string, ProbabilisticModelPtr> &_models,  const ProbabilisticModelParameters & parameters) {
        ProbabilisticModelParameterValuePtr modelpar =
            parameters.getOptionalParameterValue(model_name);

        if (modelpar == NULL) {
            std::cerr << "ERROR:: Missing definition of the model  "
                      << model_name << std::endl;
            return;
        }
        if (_models.find(model_name) != _models.end()) {
            return;
        }

        std::string cfg = modelpar->getString();
        ProbabilisticModelCreatorClient creator;
        ConfigurationReader modelreader;
        if ((cfg.size()) > 0 && (cfg[0] == '[')) {
            cfg = cfg.substr(1, cfg.size() - 2);
            if (modelreader.load(cfg)) {
                ProbabilisticModelParameterValuePtr modelpar =  (modelreader.parameters())->getOptionalParameterValue("model");
                if(modelpar != NULL)
                    {
                        std::string submodelstr = modelpar->getString();
                        if (!((submodelstr.size()) > 0 && (submodelstr[0] == '['))) {
                            restore_submodel(submodelstr, _models, parameters);
                        }
                    }
                ProbabilisticModelPtr m = creator.create(*(modelreader.parameters()), _models);
                _models[model_name] = m;
            } else{
                std::cerr << "/=======/\n" << cfg << "/========/" << std::endl;
                exit(-1);
            }
        }
        else
            {
                ProbabilisticModelPtr m = creator.create(cfg);
                if (m == NULL) {
                    std::cerr << "Can not load model " << cfg << "! " << std::endl;
                    return;
                }
                _models[model_name] = m;
            }
    }


    double MultipleSequentialModel::prefix_sum_array_compute(int begin, int end, int phase) {
        if(begin < 0)
            return -HUGE;
        if(begin > end)
            return -HUGE;

        double sum = -HUGE;
        int b = begin;
        int e = 0;
        for(int i = 0; i < _idx_not_limited; i++)
            {
                e = b + _max_size[i] - 1;
                if (e >= _seqsize)
                    e = _seqsize-1;
                if(i == 0)
                    sum = _sub_models[i]->prefix_sum_array_compute(b,e,phase);
                else
                    sum += _sub_models[i]->prefix_sum_array_compute(b,e,phase);
                phase = mod(phase + e - b + 1, 3);
                if( e >=  (int)end)
                    return sum;
                b = e + 1;

            }
        int begin_of_not_limited = b;
        e = end;
        for (int i = _sub_models.size()-1; i > _idx_not_limited ; i--)
            {
                b = e - _max_size[i] + 1;
                int phase2 = mod(phase + b - begin_of_not_limited + 1, 3);
                sum += _sub_models[i]->prefix_sum_array_compute(b,e,phase2);
                e = b - 1;

            }
        int end_of_not_limited = e;

        if( end_of_not_limited - begin_of_not_limited + 1 > 0 ){
            sum += _sub_models[_idx_not_limited]->prefix_sum_array_compute(begin_of_not_limited, end_of_not_limited, phase);
        }
        return sum;
    }
    double MultipleSequentialModel::prefix_sum_array_compute(int begin, int end)
    {
        return prefix_sum_array_compute(begin, end, 0);
    }
    bool MultipleSequentialModel::initialize_prefix_sum_array(const Sequence & s, int phase)
    {
        if(ProbabilisticModel::initialize_prefix_sum_array(s))
            return true;
        for(int i = 0; i < (int)_sub_models.size(); i++)
            {
                _sub_models[i]->initialize_prefix_sum_array(s);
            }
        _seqsize = s.size();
        return true;
    }

    bool MultipleSequentialModel::initialize_prefix_sum_array(const Sequence & s)
    {
        return initialize_prefix_sum_array(s, 0);
    }




    std::string MultipleSequentialModel::str() const
    {
        std::stringstream s;
        std::map <std::string, ProbabilisticModelParameterValuePtr> ::const_iterator it;
        std::map <std::string, ProbabilisticModelParameterValuePtr> p = _parameters.parameters();
        for(it = p.begin(); it != p.end(); it++)
            {
                s << it->first << " = " << (it->second)->str() << std::endl;
            }
        return s.str();
    }

    double MultipleSequentialModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end, int phase) const {
        if (end >= s.size())
            return -HUGE;
        if(begin < 0)
            return -HUGE;
        if(begin > end)
            return -HUGE;

        double sum = -HUGE;
        int b = begin;
        int e = 0;
        for(int i = 0; i < _idx_not_limited; i++)
            {
                e = b + _max_size[i] - 1;
                if (e >= s.size())
                    e = s.size()-1;
                if(i == 0)
                    sum = _sub_models[i]->evaluate(s,b,e, phase);
                else
                    sum += _sub_models[i]->evaluate(s,b,e,phase);
                phase = mod(phase + e - b + 1, 3);
                if( e >=  (int)end)
                    return sum;
                b = e + 1;
            }
        int begin_of_not_limited = b;
        e = end;
        for (int i = _sub_models.size()-1; i > _idx_not_limited ; i--)
            {
                b = e - _max_size[i] + 1;
                sum += _sub_models[i]->evaluate(s,b,e,phase);
                e = b - 1;
            }
        int end_of_not_limited = e;
        if( end_of_not_limited - begin_of_not_limited + 1 > 0 )
            sum += _sub_models[_idx_not_limited]->evaluate(s, begin_of_not_limited, end_of_not_limited, phase);
        return sum;
    }
    void MultipleSequentialModel::initialize (const ProbabilisticModelParameters & p )
    {
        ProbabilisticModelParameterValuePtr modelspar = p.getMandatoryParameterValue("models");
        ProbabilisticModelParameterValuePtr maxsizepar = p.getMandatoryParameterValue("max_length");
        ProbabilisticModelCreatorClient creator;
        StringVector modelnames = modelspar->getStringVector();
        DoubleVector maxlength = maxsizepar->getDoubleVector();

        if(maxlength.size() != modelnames.size() )
            {
                std::cerr << "ERROR: number of models does not match the number of max_length";
                exit(-1);
            }
        _sub_models.resize(modelnames.size());
        _max_size.resize(modelnames.size());
        _idx_not_limited =0;
        int count = 0;
        for(int i = 0; i < (int)modelnames.size();i++)
            {
                if(maxlength[i] == -1)
                    {

                        count++;
                        _idx_not_limited = i;
                    }
            }
        if (count != 1)
            {
                std::cerr << "ERROR: Only one model can has unlimited length\n" << std::endl;
                exit(-1);
            }
        for(int i = 0; i < (int)modelnames.size();i++)
            {
                restore_submodel(modelnames[i], _models, p);
                _sub_models[i] = _models[modelnames[i]];
                if(_sub_models[i] == NULL)
                    {
                        std::cerr << "ERROR: Cannot load model " << modelnames[i] << std::endl;
                        exit(-1);
                    }
                setAlphabet(_sub_models[i]->alphabet());
                _max_size[i] = maxlength[i];
            }
        _parameters =  p;
    }
    //! Calculates the sequence likelihood given this model
    double MultipleSequentialModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const {
        return evaluate(s, begin,end,0);
    }

    ProbabilisticModelParameters MultipleSequentialModel::parameters() const
    {
        return _parameters;
    }
}
