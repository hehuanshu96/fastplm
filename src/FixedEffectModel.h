#ifndef FASTPLM_FIXED_EFFECT_MODEL_H
#define FASTPLM_FIXED_EFFECT_MODEL_H

#include "Common.h"
#include "FixedEffects.h"
#include "LinearModel.h"

struct FixedEffectModel {
public:
    LinearModel demeaned;

    std::vector<arma::vec> sfeCoefs;
    std::vector<arma::mat> cfeCoefs;

    arma::vec fittedValues, residuals;
    double intercept;

    static const FixedEffectModel solve(const arma::mat& data, const FixedEffects& fixedEffects);
    static const FixedEffectModel solve(const arma::mat& X, const arma::vec& Y, const FixedEffects& fixedEffects) {
        return solve(arma::join_horiz(Y, X), fixedEffects);
    }

#ifndef BUILD_WITHOUT_R
    operator Rcpp::List() const {
        Rcpp::List _;
        _["demeaned"] = static_cast<Rcpp::List>(demeaned);
        _["coefficients"] = demeaned.beta;

        Rcpp::List sfeCoefs_;
        for (const auto& coefs : sfeCoefs)
            sfeCoefs_.push_back(coefs);
        _["sfe.coefs"] = sfeCoefs_;
        Rcpp::List cfeCoefs_;
        for (const auto& coefs : cfeCoefs)
            cfeCoefs_.push_back(coefs);
        _["cfe.coefs"] = cfeCoefs_;

        _["fitted.values"] = fittedValues;
        _["residuals"] = residuals;
        _["intercept"] = intercept;

        return _;
    }
#endif
};

#endif
