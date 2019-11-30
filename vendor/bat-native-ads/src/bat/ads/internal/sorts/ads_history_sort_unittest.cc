/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/filters/ad_history_sort_factory.h"
#include "bat/ads/internal/client_mock.h"
#include "bat/ads/internal/ads_client_mock.h"
#include "bat/ads/internal/ads_impl.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

class BatAdsHistorySortTest : public ::testing::Test {
 protected:
  BatAdsHistorySortTest()
      : mock_ads_client_(std::make_unique<MockAdsClient>()),
        ads_(std::make_unique<AdsImpl>(mock_ads_client_.get())) {
    // You can do set-up work for each test here
  }

  ~BatAdsHistorySortTest() override {
    // You can do clean-up work that doesn't throw exceptions here
  }

  // If the constructor and destructor are not enough for setting up and
  // cleaning up each test, you can use the following methods

  void SetUp() override {
    // Code here will be called immediately after the constructor (right before
    // each test)

    ads_->Initialize(base::NullCallback());

    client_mock_ =
        std::make_unique<ClientMock>(ads_.get(), mock_ads_client_.get());

    ad_history_filter_ = std::make_unique<AdHistoryAscendingSort>();

    ads_history_.clear();
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right before the
    // destructor)
  }

  std::unique_ptr<MockAdsClient> mock_ads_client_;
  std::unique_ptr<AdsImpl> ads_;

  std::unique_ptr<ClientMock> client_mock_;
};

TEST_F(BatAdsHistorySortTest,
    NoSortOrder) {
  // Arrange

  // Act

  // Assert
  FAILED();
}

TEST_F(BatAdssHistorySortTest,
    AscendingSortOrder) {
  // Arrange

  // Act

  // Assert
  FAILED();
}

TEST_F(BatAdsHistorySortTest,
    DescendingSortOrder) {
  // Arrange

  // Act

  // Assert
  FAILED();
}

}  // namespace ads
