// SPDX-License-Identifier: GPLv3-or-later
// Copyright (C) 2020 Jesse Chappell


#include "JuceHeader.h"
#include "VersionInfo.h"

std::unique_ptr<VersionInfo> VersionInfo::fetchFromUpdateServer (const String& versionString)
{
    return fetch ("tags/" + versionString);
}

std::unique_ptr<VersionInfo> VersionInfo::fetchLatestFromUpdateServer()
{
    return fetch ("latest");
}

std::unique_ptr<InputStream> VersionInfo::createInputStreamForAsset (const Asset& asset, int& statusCode)
{
    URL downloadUrl (asset.url);
    StringPairArray responseHeaders;

    return std::unique_ptr<InputStream> (downloadUrl.createInputStream (false, nullptr, nullptr,
                                                                        "Accept: application/octet-stream",
                                                                        0, &responseHeaders, &statusCode, 1));
}

bool VersionInfo::isNewerVersionThanCurrent()
{
    jassert (versionString.isNotEmpty());

    auto currentTokens = StringArray::fromTokens (ProjectInfo::versionString, ".", {});
    auto thisTokens    = StringArray::fromTokens (versionString, ".", {});

    jassert (thisTokens.size() == 3 && thisTokens.size() == 3);

    if (currentTokens[0].getIntValue() == thisTokens[0].getIntValue())
    {
        if (currentTokens[1].getIntValue() == thisTokens[1].getIntValue())
            return currentTokens[2].getIntValue() < thisTokens[2].getIntValue();

        return currentTokens[1].getIntValue() < thisTokens[1].getIntValue();
    }

    return currentTokens[0].getIntValue() < thisTokens[0].getIntValue();
}

std::unique_ptr<VersionInfo> VersionInfo::fetch (const String& endpoint)
{
    URL latestVersionURL ("https://api.github.com/repos/essej/sonobus/releases/" + endpoint);
    std::unique_ptr<InputStream> inStream (latestVersionURL.createInputStream (false));

    if (inStream == nullptr)
        return nullptr;

    auto content = inStream->readEntireStreamAsString();
    auto latestReleaseDetails = JSON::parse (content);

    auto* json = latestReleaseDetails.getDynamicObject();

    if (json == nullptr)
        return nullptr;

    auto versionString = json->getProperty ("tag_name").toString();

    if (versionString.isEmpty())
        return nullptr;

    auto* assets = json->getProperty ("assets").getArray();

    if (assets == nullptr)
        return nullptr;

    auto releaseNotes = json->getProperty ("body").toString();
    std::vector<VersionInfo::Asset> parsedAssets;

    for (auto& asset : *assets)
    {
        if (auto* assetJson = asset.getDynamicObject())
        {
            parsedAssets.push_back ({ assetJson->getProperty ("name").toString(),
                                      assetJson->getProperty ("url").toString() });
            jassert (parsedAssets.back().name.isNotEmpty());
            jassert (parsedAssets.back().url.isNotEmpty());
        }
        else
        {
            jassertfalse;
        }
    }

    return std::unique_ptr<VersionInfo> (new VersionInfo ({ versionString, releaseNotes, std::move (parsedAssets) }));
}
