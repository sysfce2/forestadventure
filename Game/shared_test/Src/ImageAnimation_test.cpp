/*
 *	Copyright (C) 2024 Anders Wennmo
 *	This file is part of forestadventure which is released under MIT license.
 *	See file LICENSE for full license details.
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Animation/ImageAnimation.h"
#include "Mock/SequenceMock.h"
#include "SpriteMock.h"
#include "TextureMock.h"

using namespace testing;

namespace FA {

namespace Shared {

class ImageAnimationTest : public testing::Test
{
protected:
    ImageAnimationTest()
        : seqMock_(std::make_shared<StrictMock<SequenceMock<Shared::ImageFrame>>>())
        , spriteMock_(std::make_shared<StrictMock<Graphic::SpriteMock>>())
    {}

    Graphic::TextureMock textureMock_;
    sf::IntRect rect_{0, 0, 10, 12};
    ImageFrame frame_{&textureMock_, rect_, {5, 6}};
    std::shared_ptr<StrictMock<Graphic::SpriteMock>> spriteMock_;
    std::shared_ptr<StrictMock<SequenceMock<Shared::ImageFrame>>> seqMock_;
};

TEST_F(ImageAnimationTest, UpdateWithEmptyContentShouldDoNothing)
{
    EXPECT_CALL(*seqMock_, IsEmpty).WillOnce(Return(true));
    ImageAnimation animation_(spriteMock_, seqMock_);
    animation_.Update(0.01f);
}

TEST_F(ImageAnimationTest, UpdateWithContentShoulSetTexture)
{
    EXPECT_CALL(*seqMock_, IsEmpty).WillOnce(Return(false));
    ImageAnimation animation(spriteMock_, seqMock_);
    EXPECT_CALL(*seqMock_, Update(0.01f));
    EXPECT_CALL(*seqMock_, GetCurrent).WillOnce(Return(frame_));
    EXPECT_CALL(*spriteMock_, setTextureImpl(Address(&textureMock_), false));
    EXPECT_CALL(*spriteMock_, setTextureRect(Eq(rect_)));
    animation.Update(0.01f);
}

TEST_F(ImageAnimationTest, UpdateWithContentAndCenterShoulSetTextureAndSetOrigin)
{
    EXPECT_CALL(*seqMock_, IsEmpty).WillOnce(Return(false));
    ImageAnimation animation(spriteMock_, seqMock_);
    EXPECT_CALL(*seqMock_, Update(0.01f));
    EXPECT_CALL(*seqMock_, GetCurrent).WillOnce(Return(frame_));
    EXPECT_CALL(*spriteMock_, setTextureImpl(Address(&textureMock_), false));
    EXPECT_CALL(*spriteMock_, setTextureRect(Eq(rect_)));
    EXPECT_CALL(*spriteMock_, setOrigin(5, 6));
    animation.Center();
    animation.Update(0.01f);
}

TEST_F(ImageAnimationTest, IntersectsWithIntersectingSpritesShouldReturnTrue)
{
    EXPECT_CALL(*seqMock_, IsEmpty).WillOnce(Return(false));
    ImageAnimation animation(spriteMock_, seqMock_);
    auto otherSeqMock = std::make_shared<StrictMock<SequenceMock<Shared::ImageFrame>>>();
    auto otherSpriteMock = std::make_shared<StrictMock<Graphic::SpriteMock>>();
    EXPECT_CALL(*otherSeqMock, IsEmpty).WillOnce(Return(false));
    ImageAnimation other(otherSpriteMock, otherSeqMock);
    sf::FloatRect otherRect{0, 0, 10, 10};
    sf::FloatRect rect{2, 2, 2, 2};
    EXPECT_CALL(*spriteMock_, getGlobalBounds).WillOnce(Return(rect));
    EXPECT_CALL(*otherSpriteMock, getGlobalBounds).WillOnce(Return(otherRect));

    bool result = animation.Intersects(other);
    EXPECT_TRUE(result);
}

TEST_F(ImageAnimationTest, RegisterUpdateCBShouldBeCalledDuringUpdate)
{
    EXPECT_CALL(*seqMock_, IsEmpty).WillOnce(Return(false));
    ImageAnimation animation(spriteMock_, seqMock_);
    MockFunction<void(const ImageAnimationIf&)> callbackFunctionMock;
    animation.RegisterUpdateCB(callbackFunctionMock.AsStdFunction());
    EXPECT_CALL(*seqMock_, Update(0.01f));
    EXPECT_CALL(*seqMock_, GetCurrent).WillOnce(Return(frame_));
    EXPECT_CALL(*spriteMock_, setTextureImpl(Address(&textureMock_), false));
    EXPECT_CALL(*spriteMock_, setTextureRect(Eq(rect_)));
    EXPECT_CALL(callbackFunctionMock, Call(Ref(animation)));
    animation.Update(0.01f);
}

}  // namespace Shared

}  // namespace FA
