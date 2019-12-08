#include <QtTest/QtTest>

#include <gui/input.h>

class TestInputMac : public QObject
{
	Q_OBJECT

private slots:
	void LessThanModifierKeys() noexcept;
	void SpecialKeys() noexcept;
};

void TestInputMac::LessThanModifierKeys() noexcept
{
	QKeyEvent evLessThanControl{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::ControlModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanControl), QString{ "<D-lt>" });

	QKeyEvent evLessThanAlt{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::AltModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanAlt), QString{ "<A-lt>" });

	QKeyEvent evLessThanMeta{ QEvent::KeyPress, Qt::Key::Key_Less, Qt::ShiftModifier | Qt::MetaModifier, "<" };
	QCOMPARE(NeovimQt::Input::convertKey(evLessThanMeta), QString{ "<C-lt>" });
}

void TestInputMac::SpecialKeys() noexcept
{
	const QMap<int, QString>& specialKeys { NeovimQt::Input::GetSpecialKeysMap() };

	for (const auto k : specialKeys.keys()) {
		// On Mac Meta is the Control key, treated as C-.
		QList<QPair<QKeyEvent, QString>> keyEventList{
			{ { QEvent::KeyPress, k, Qt::NoModifier, {} },      "<%1>" },
			{ { QEvent::KeyPress, k, Qt::ControlModifier, {} }, "<D-%1>" },
			{ { QEvent::KeyPress, k, Qt::AltModifier, {} },     "<A-%1>" },
			{ { QEvent::KeyPress, k, Qt::MetaModifier, {} },    "<C-%1>" },
		};

		for (const auto& keyEvent : keyEventList) {
			QCOMPARE(NeovimQt::Input::convertKey(keyEvent.first),
				keyEvent.second.arg(specialKeys.value(k)));
		}
	}
}

#include "tst_input_mac.moc"
QTEST_MAIN(TestInputMac)
