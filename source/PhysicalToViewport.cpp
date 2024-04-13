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
    return juce::Point<float> {
        juce::jmap (in.getX(), mExpandedPhysicalBounds.getX(), mExpandedPhysicalBounds.getRight(), 0.f, float (mViewportBounds.getWidth())),
        juce::jmap (in.getY(), mExpandedPhysicalBounds.getY(), mExpandedPhysicalBounds.getBottom(), float (mViewportBounds.getHeight()), 0.f)
    }
        .roundToInt();
}

juce::Rectangle<int> PhysicalToViewport::transform (juce::Rectangle<float> in)
{
    return {
        transform (in.getTopLeft()),
        transform (in.getBottomRight())
    };
}
void PhysicalToViewport::updateExpandedPhysicalBounds()
{
    float viewportAspectRatio = mViewportBounds.toFloat().getAspectRatio();
    float physicalAspectRatio = mPhysicalBounds.getAspectRatio();
    float aspectRatioRatio = physicalAspectRatio / viewportAspectRatio;
    /**
     * The mPhysicalBounds expanded to match the aspect ratio of the viewport
     */
    mExpandedPhysicalBounds = aspectRatioRatio < 1
                                  ? mPhysicalBounds.withSizeKeepingCentre (mPhysicalBounds.getWidth() / aspectRatioRatio, mPhysicalBounds.getHeight())
                                  : mPhysicalBounds.withSizeKeepingCentre (mPhysicalBounds.getWidth(), mPhysicalBounds.getHeight() * aspectRatioRatio);
}
