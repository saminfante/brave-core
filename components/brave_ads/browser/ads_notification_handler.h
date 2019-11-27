/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_BROWSER_ADS_NOTIFICATION_HANDLER_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_BROWSER_ADS_NOTIFICATION_HANDLER_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "base/supports_user_data.h"
#include "chrome/browser/notifications/notification_handler.h"

class Profile;

namespace brave_ads {

class AdsServiceImpl;

const void* const kAdsNotificationHandlerKey = &kAdsNotificationHandlerKey;

class AdsNotificationHandler : public NotificationHandler,
                               public base::SupportsUserData::Data {
 public:
  AdsNotificationHandler(Profile* profile);
  ~AdsNotificationHandler() override;

  // NotificationHandler implementation
  void OnShow(
      Profile* profile,
      const std::string& id) override;
  void OnClose(
      Profile* profile,
      const GURL& origin,
      const std::string& id,
      bool by_user,
      base::OnceClosure completed_closure) override;
  void OnClick(
      Profile* profile,
      const GURL& origin,
      const std::string& id,
      const base::Optional<int>& action_index,
      const base::Optional<base::string16>& reply,
      base::OnceClosure completed_closure) override;
  void DisableNotifications(
      Profile* profile,
      const GURL& origin) override;
  void OpenSettings(
      Profile* profile,
      const GURL& origin) override;

  void SetAdsService(brave_ads::AdsServiceImpl* ads_service);

  static const void* UserDataKey();

 private:
  // This class is used to set the AdsNotificationHandler as user data
  // on a profile. This is used instead of AdsNotificationHandler
  // directly because SetUserData requires a std::unique_ptr. This is
  // safe because we remove the user data in AdsNotificationHandler's
  // destructor.
  class UnownedPointer : public base::SupportsUserData::Data {
   public:
    explicit UnownedPointer(AdsNotificationHandler* pointer)
        : pointer_(pointer) {}

    AdsNotificationHandler* get() const { return pointer_; }

   private:
    AdsNotificationHandler* const pointer_;

    DISALLOW_COPY_AND_ASSIGN(UnownedPointer);
  };

  Profile* profile_;
  base::WeakPtr<brave_ads::AdsServiceImpl> ads_service_;

  DISALLOW_COPY_AND_ASSIGN(AdsNotificationHandler);
};

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_BROWSER_ADS_SERVICE_IMPL_H_
