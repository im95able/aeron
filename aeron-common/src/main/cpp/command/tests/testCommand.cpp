/*
 * Copyright 2014 Real Logic Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <array>
#include <gtest/gtest.h>
#include <concurrent/AtomicBuffer.h>
#include <command/ConnectionMessageFlyweight.h>
#include <command/ConnectionReadyFlyweight.h>
#include <command/RemoveMessageFlyweight.h>
#include <command/SubscriptionMessageFlyweight.h>
#include <command/PublicationMessageFlyweight.h>
#include <command/PublicationReadyFlyweight.h>

using namespace aeron::common::command;
using namespace aeron::common::concurrent;

static std::array<std::uint8_t, 1024> testBuffer;

static void clearBuffer()
{
    testBuffer.fill(0);
}

TEST (commandTests, testInstantiateFlyweights)
{
    clearBuffer();
    AtomicBuffer ab (&testBuffer[0], testBuffer.size());
    const size_t BASEOFFSET = 256;

    std::string channelData = "channelData";

	ASSERT_NO_THROW({
		ConnectionMessageFlyweight cmd(ab, BASEOFFSET);
	});

    ASSERT_NO_THROW({
        ConnectionReadyFlyweight cmd(ab, BASEOFFSET);
    });

	ASSERT_NO_THROW({
		RemoveMessageFlyweight cmd(ab, BASEOFFSET);
	});

	ASSERT_NO_THROW({
		SubscriptionMessageFlyweight cmd(ab, BASEOFFSET);
	});

	ASSERT_NO_THROW({
		PublicationMessageFlyweight cmd(ab, BASEOFFSET);
	});

	ASSERT_NO_THROW({
		PublicationReadyFlyweight cmd(ab, BASEOFFSET);
	});
}

TEST (commandTests, testConnectionMessageFlyweight)
{
    clearBuffer();
    AtomicBuffer ab (&testBuffer[0], testBuffer.size());
    const size_t BASEOFFSET = 256;

    std::string channelData = "channelData";

    ASSERT_NO_THROW({
        ConnectionMessageFlyweight cmd (ab, BASEOFFSET);
        cmd.correlationId(1).sessionId(2).streamId(3).channel(channelData);

        ASSERT_EQ(ab.getInt64(BASEOFFSET + 0), 1);
        ASSERT_EQ(ab.getInt32(BASEOFFSET + 8), 2);
        ASSERT_EQ(ab.getInt32(BASEOFFSET + 12), 3);
        ASSERT_EQ(ab.getStringUtf8(BASEOFFSET + 16), channelData);

        ASSERT_EQ(cmd.correlationId(), 1);
        ASSERT_EQ(cmd.sessionId(), 2);
        ASSERT_EQ(cmd.streamId(), 3);
        ASSERT_EQ(cmd.channel(), channelData);

        ASSERT_EQ(cmd.length(), offsetof(ConnectionMessageDefn, channel.channelData) + channelData.length());
    });
}
