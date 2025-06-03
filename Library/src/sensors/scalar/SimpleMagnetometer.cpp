/*
    This file is a part of Stonefish.

    Stonefish is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Stonefish is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//
//  SimpleMagnetometer.cpp
//  Stonefish
//
//  Created by Luca Istrate on 02/06/2025.
//  Copyright (c) 2014-2025 Patryk Cieslak. All rights reserved.
//

#include "sensors/scalar/SimpleMagnetometer.h"

#include "entities/MovingEntity.h"
#include "sensors/Sample.h"
#include "core/SimulationApp.h"
#include "core/SimulationManager.h"

namespace sf
{

SimpleMagnetometer::SimpleMagnetometer(std::string uniqueName, Scalar frequency, int historyLength) : LinkSensor(uniqueName, frequency, historyLength)
{
    channels.push_back(SensorChannel("Mag X", QuantityType::MAGNETIC_FLUX_DENSITY));
    channels.push_back(SensorChannel("Mag Y", QuantityType::MAGNETIC_FLUX_DENSITY));
    channels.push_back(SensorChannel("Mag Z", QuantityType::MAGNETIC_FLUX_DENSITY));
}

void SimpleMagnetometer::InternalUpdate(Scalar dt)
{
    //get sensor frame in world
    Transform magTrans = getSensorFrame();

    // TODO setNorth member function?
    Vector3 north{1, 0, 0};
    Vector3 mag = magTrans.getBasis().inverse() * north;  // should be normalized
    // TODO scale to something plausible

    //record sample
    Sample s{std::vector<Scalar>({mag.x(), mag.y(), mag.z()})};
    AddSampleToHistory(s);
}

void SimpleMagnetometer::Reset()
{
    ScalarSensor::Reset();
}

void SimpleMagnetometer::setNoise(Vector3 magStdDev)
{
    channels[0].setStdDev(btClamped(magStdDev.x(), Scalar(0), Scalar(BT_LARGE_FLOAT)));
    channels[1].setStdDev(btClamped(magStdDev.y(), Scalar(0), Scalar(BT_LARGE_FLOAT)));
    channels[2].setStdDev(btClamped(magStdDev.z(), Scalar(0), Scalar(BT_LARGE_FLOAT)));
}

ScalarSensorType SimpleMagnetometer::getScalarSensorType() const
{
    return ScalarSensorType::SIMPLE_MAGNETOMETER;
}

}
