// SPDX-License-Identifier: GPLv3-or-later
// Copyright (C) 2020 Jesse Chappell

//
//  SonobusTypes.h
//  SonoBus
//
//  Created by Jesse Chappell on 8/5/20.
//  Copyright © 2020 Sonosaurus. All rights reserved.
//

#pragma once


class SonobusCommands
{
public:
    
    enum {
        MuteAllInput = 1,
        MuteAllPeers,
        TogglePlayPause,
        ToggleLoop,
        TrimSelectionToNewFile,
        CloseFile,
        Connect,
        Disconnect,
        ShareFile,
        RevealFile,
        ShowOptions,
        OpenFile,
        RecordToggle,
        CheckForNewVersion
    };
    
};
