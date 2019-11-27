/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/browser/ads_notification_handler.h"

#include <utility>

#include "brave/components/brave_ads/browser/ads_service_impl.h"
#include "chrome/browser/profiles/profile.h"

namespace brave_ads {

int kUserDataKey;  // The value is not important, the address is a key.

AdsNotificationHandler::AdsNotificationHandler(Profile* profile)
    : profile_(profile) {
  DCHECK(profile);
  profile_->SetUserData(UserDataKey(), std::make_unique<UnownedPointer>(this));
}

AdsNotificationHandler::~AdsNotificationHandler() {
  profile_->SetUserData(UserDataKey(), nullptr);
}

void AdsNotificationHandler::OnShow(
    Profile* profile,
    const std::string& id) {
  if (!ads_service_) {
    return;
  }

  ads_service_->OnShow(profile, id);
}

void AdsNotificationHandler::OnClose(
    Profile* profile,
    const GURL& origin,
    const std::string& id,
    bool by_user,
    base::OnceClosure completed_closure) {
  if (!ads_service_) {
    if (completed_closure) {
      std::move(completed_closure).Run();
    }

    return;
  }

  ads_service_->OnClose(profile, origin, id, by_user,
                        std::move(completed_closure));
}

void AdsNotificationHandler::OnClick(
    Profile* profile,
    const GURL& origin,
    const std::string& id,
    const base::Optional<int>& action_index,
    const base::Optional<base::string16>& reply,
    base::OnceClosure completed_closure) {
  if (!ads_service_) {
    return;
  }

  ads_service_->ViewAd(id);
}

void AdsNotificationHandler::DisableNotifications(
    Profile* profile,
    const GURL& origin) {
}

void AdsNotificationHandler::OpenSettings(
      Profile* profile,
      const GURL& origin) {
  if (!ads_service_) {
    return;
  }

  DCHECK(origin.has_query());

  auto id = origin.query();
  ads_service_->ViewAd(id);
}

void AdsNotificationHandler::SetAdsService(
    brave_ads::AdsServiceImpl* ads_service) {
  if (ads_service) {
    ads_service_ = ads_service->AsWeakPtr();
  } else {
    ads_service = nullptr;
  }
}

// static
const void* AdsNotificationHandler::UserDataKey() {
  return &kUserDataKey;
}

}  // namespace brave_ads
