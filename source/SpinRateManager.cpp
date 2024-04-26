//
// Created by Henry Ives on 4/19/24.
//

#include "SpinRateManager.h"
#include "PluginProcessor.h"
#include <array>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>

std::string exec (const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype (&pclose)> pipe (popen (cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error ("popen() failed!");
    }
    while (fgets (buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

SpinRateManager::SpinRateManager (PluginProcessor* pp) : mPluginProcessor (pp)
{
    updateAvailableSources();
}

void SpinRateManager::updateParams()
{
    mManualSpinRate = dynamic_cast<juce::AudioParameterFloat*> (mPluginProcessor->getVTS().getParameter ("spin_rate"));
    jassert (mManualSpinRate != nullptr);
}

void SpinRateManager::setSource (int source)
{
    jassert (source >= 0 && source < mAvailableSources.size());
    auto newSource = mAvailableSources[source].first;
    DBG ("Set Spin Rate Source");
    DBG (newSource);
    if (newSource.toStdString() != MANUAL_SOURCE.second)
    {
        try
        {
            mSerialPort = std::unique_ptr<SerialPort> (new SerialPort { newSource.toStdString() });
            mCurrentPort = source;
        } catch (const std::exception& e)
        {
            DBG ("Error opening serial port");
            DBG (e.what());
            mPluginProcessor->getSpinRateManager()->setSource (MANUAL_SOURCE.first);
            mCurrentPort = MANUAL_SOURCE.first;
            onSourceChange();
        }
    }
    else
    {
        mSerialPort = nullptr;
        mCurrentPort = MANUAL_SOURCE.first;
    }
}

void SpinRateManager::updateAvailableSources()
{
    try
    {
        std::string externalSourcesString = exec ("ls /dev/tty.* | grep usbmodem");
        DBG ("External Sources found:");
        DBG (externalSourcesString);

        // Split exec result into list
        std::vector<std::pair<std::string, bool>> sourceList;
        auto ss = std::stringstream { externalSourcesString };
        for (std::string line; std::getline (ss, line, '\n');)
        {
            try
            {
                auto tmp = SerialPort (line);
                sourceList.push_back (std::make_pair (line, true));
            } catch (const std::exception& e)
            {
                sourceList.push_back (std::make_pair (line, false));
            }
        }
        sourceList.insert (sourceList.begin() + MANUAL_SOURCE.first, std::make_pair (MANUAL_SOURCE.second, true));
        mAvailableSources.clear();
        for (auto& source : sourceList)
        {
            mAvailableSources.push_back (std::make_pair (source.first, source.second));
        }
    } catch (const std::exception& e)
    {
        DBG ("Error updating available sources");
        DBG (e.what());
        mAvailableSources.clear();
        mAvailableSources.insert (mAvailableSources.begin() + MANUAL_SOURCE.first, std::make_pair (MANUAL_SOURCE.second, true));
    }
}
void SpinRateManager::processBlock (int numSamples)
{
    mSmoothedSpinRate.skip (numSamples);
}

void SpinRateManager::readValues()
{
    if (mSerialPort == nullptr)
    {
        DBG ("Manual value");
        DBG (mManualSpinRate->get());
        mSmoothedSpinRate.setTargetValue (mManualSpinRate->get());
    }
    else
    {
        auto line = mSerialPort->readLastLine();
        DBG ("Read Line");
        DBG (line);
        if (!line.empty())
        {
            try
            {
                float mostRecentlySentRpm = std::stof (line);
                float newRps = mostRecentlySentRpm / 60.f;
                DBG ("Read new RPS: " + std::to_string (newRps));
                mSmoothedSpinRate.setTargetValue (newRps);
            } catch (const std::invalid_argument e)
            {
                DBG ("No line read");
            }
        }
    }
}

void SpinRateManager::prepareToPlay (double sampleRate)
{
    mSmoothedSpinRate.reset (sampleRate, 0.1f);
}
