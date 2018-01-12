#pragma once

#include "GameSparks/Private/GameSparksComponent.h"
#include "Private/GameSparksObject.h"
#include "Private/GSMessageListenersObject.h"
#include "GameSparksModule.h"
#include <GameSparks/GS.h>
#include <GameSparks/generated/GSResponses.h>
#include <GameSparks/generated/GSRequests.h>

typedef GameSparks::Core::GS GS_Global; //( ͡° ͜ʖ ͡°)
typedef GameSparks::Api::Responses::AuthenticationResponse GS_AuthResponse; // ( ͡° ͜ʖ ͡°)
typedef GameSparks::Api::Requests::AuthenticationRequest GS_AuthRequest;
typedef GameSparks::Api::Requests::AccountDetailsRequest GS_AccountDetails;