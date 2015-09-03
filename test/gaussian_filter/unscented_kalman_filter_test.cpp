/*
 * This is part of the FL library, a C++ Bayesian filtering library
 * (https://github.com/filtering-library)
 *
 * Copyright (c) 2014 Jan Issac (jan.issac@gmail.com)
 * Copyright (c) 2014 Manuel Wuthrich (manuel.wuthrich@gmail.com)
 *
 * Max-Planck Institute for Intelligent Systems, AMD Lab
 * University of Southern California, CLMC Lab
 *
 * This Source Code Form is subject to the terms of the MIT License (MIT).
 * A copy of the license can be found in the LICENSE file distributed with this
 * source code.
 */

/**
 * \file gaussian_filter_kf_test.cpp
 * \date Febuary 2015
 * \author Jan Issac (jan.issac@gmail.com)
 */

#include <gtest/gtest.h>
#include "../typecast.hpp"

#include <Eigen/Dense>

#include "gaussian_filter_test_suite.hpp"
#include <fl/util/meta.hpp>
#include <fl/filter/gaussian/gaussian_filter.hpp>

using namespace fl;

template <
    int StateDimension,
    int InputDimension,
    int ObsrvDimension,
    int FilterIterations = 100
>
struct UnscentedKalmanFilterTestConfiguration
{
    enum : signed int
    {
        StateDim = StateDimension,
        InputDim = InputDimension,
        ObsrvDim = ObsrvDimension,
        Iterations = FilterIterations
    };

    template <typename ModelFactory>
    struct FilterDefinition
    {
        typedef UnscentedQuadrature Quadrature;

        typedef GaussianFilter<
                        typename ModelFactory::LinearStateTransition,
                        typename ModelFactory::LinearObservation,
                        Quadrature
                > Type;
    };

    template <typename ModelFactory>
    static typename FilterDefinition<ModelFactory>::Type
    create_filter(ModelFactory&& factory)
    {
        return typename FilterDefinition<ModelFactory>::Type(
            factory.create_linear_state_model(),
            factory.create_observation_model(),
            UnscentedQuadrature());
    }
};

typedef ::testing::Types<
            StaticTest<UnscentedKalmanFilterTestConfiguration<3, 1, 2>>,
            StaticTest<UnscentedKalmanFilterTestConfiguration<3, 3, 10>>,
            StaticTest<UnscentedKalmanFilterTestConfiguration<10, 10, 20>>,
            DynamicTest<UnscentedKalmanFilterTestConfiguration<3, 1, 2>>,
            DynamicTest<UnscentedKalmanFilterTestConfiguration<3, 3, 10>>,
            DynamicTest<UnscentedKalmanFilterTestConfiguration<10, 10, 20>>
        > TestTypes;

INSTANTIATE_TYPED_TEST_CASE_P(UnscentedKalmanFilterTest,
                              GaussianFilterTest,
                              TestTypes);
