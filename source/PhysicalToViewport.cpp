//
// Created by Henry Ives on 4/12/24.
//

#include "PhysicalToViewport.h"

PhysicalToViewport::PhysicalToViewport (juce::Rectangle<float> physicalBounds, juce::Rectangle<int> viewportBounds) : mPhysicalBounds (physicalBounds),
                                                                                                                      mViewportBounds (viewportBounds) {}

PhysicalToViewport::PhysicalToViewport() : mPhysicalBounds (juce::Rectangle<float> {}.withCentre ({ 0, 0 }).withSizeKeepingCentre (MAX_SPINNER_DIAMETER * 1.1, std::max (MAX_SPINNER_DIAMETER / 2, MAX_DISTANCE_TO_FOCAL_POINT) * 2.2)),
                                           mViewportBounds (juce::Rectangle<int> { 200, 200 }) {}

juce::Point<int> PhysicalToViewport::transform (juce::Point<float> in)
{
    float viewportAspectRatio = mViewportBounds.toFloat().getAspectRatio();
    float physicalAspectRatio = mPhysicalBounds.getAspectRatio();
    float aspectRatioRatio = physicalAspectRatio / viewportAspectRatio;
    /**
     * The mPhysicalBounds expanded to match the aspect ratio of the viewport
     */
    juce::Rectangle<float> expandedBounds = aspectRatioRatio < 1
                                                ? mPhysicalBounds.withSizeKeepingCentre (mPhysicalBounds.getWidth() / aspectRatioRatio, mPhysicalBounds.getHeight())
                                                : mPhysicalBounds.withSizeKeepingCentre (mPhysicalBounds.getWidth(), mPhysicalBounds.getHeight() * aspectRatioRatio);
    return juce::Point<float> {
        juce::jmap (in.getX(), expandedBounds.getX(), expandedBounds.getRight(), 0.f, float (mViewportBounds.getWidth())),
        juce::jmap (in.getY(), expandedBounds.getY(), expandedBounds.getBottom(), float (mViewportBounds.getHeight()), 0.f)
    }
        .roundToInt();
}