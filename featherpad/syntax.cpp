/*
 * Copyright (C) Pedram Pourang (aka Tsu Jan) 2014 <tsujan2000@gmail.com>
 *
 * FeatherPad is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FeatherPad is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

#include "singleton.h"
#include "ui_fp.h"
#include <QMimeDatabase>
#include <QFileInfo>

namespace FeatherPad {

static QString getMimeType (const QString &fname)
{
    QMimeDatabase mimeDatabase;
    QMimeType mimeType = mimeDatabase.mimeTypeForFile (QFileInfo (fname));
    return mimeType.name();
}
/*************************/
void FPwin::toggleSyntaxHighlighting()
{
    int count = ui->tabWidget->count();
    if (count == 0) return;

    for (int i = 0; i < count; ++i)
    {
        syntaxHighlighting (qobject_cast< TabPage *>(ui->tabWidget->widget (i))->textEdit(),
                            ui->actionSyntax->isChecked());
    }
}
/*************************/
void FPwin::setProgLang (TextEdit *textEdit)
{
    if (textEdit == nullptr) return;

    QString fname = textEdit->getFileName();
    if (fname.isEmpty()) return;

    if (fname.endsWith (".sub"))
        return;

    QString progLan;

    /* first check some endings */
    QRegExp rx ("*.*");
    rx.setPatternSyntax (QRegExp::Wildcard);
    QString baseName = fname.section ('/', -1);
    if (rx.exactMatch (baseName))
    {
        if (fname.endsWith (".cpp") || fname.endsWith (".h"))
            progLan = "cpp";
        else if (fname.endsWith (".c"))
            progLan = "c";
        else if (fname.endsWith (".sh")
                 || baseName == ".bashrc" || baseName == ".bash_profile" || baseName == ".bash_functions"
                 || baseName == ".xprofile" || baseName == ".profile"
                 || baseName == ".bash_aliases" || baseName == ".mkshrc"
                 || baseName == ".zprofile" || baseName == ".zlogin"
                 || baseName == ".zshrc" || baseName == ".zshenv")
            progLan = "sh";
        else if (fname.endsWith (".rb"))
            progLan = "ruby";
        else if (fname.endsWith (".lua"))
            progLan = "lua";
        else if (fname.endsWith (".py"))
            progLan = "python";
        else if (fname.endsWith (".pl"))
            progLan = "perl";
        else if (fname.endsWith (".pro") || fname.endsWith (".pri"))
            progLan = "qmake";
        else if (fname.endsWith (".tr") || fname.endsWith (".t") || fname.endsWith (".roff"))
            progLan = "troff";
        else if (fname.endsWith (".xml", Qt::CaseInsensitive) || fname.endsWith (".svg", Qt::CaseInsensitive) || fname.endsWith (".qrc")
                 || fname.endsWith (".meta4", Qt::CaseInsensitive) || fname.endsWith (".metalink", Qt::CaseInsensitive)
                 /*|| fname.endsWith (".ui")*/ || fname.endsWith (".rdf") || fname.endsWith (".docbook") || fname.endsWith (".fnx")
                 || fname.endsWith (".ts") || fname.endsWith (".menu")
                 || fname.endsWith (".xspf", Qt::CaseInsensitive) || fname.endsWith (".asx", Qt::CaseInsensitive))
            progLan = "xml";
        else if (fname.endsWith (".css") || fname.endsWith (".qss"))
            progLan = "css";
        else if (fname.endsWith (".desktop") || fname.endsWith (".desktop.in") || fname.endsWith (".directory"))
             progLan = "desktop";
        else if (fname.endsWith (".conf") || fname.endsWith (".ini") || fname.endsWith (".kvconfig")
                 || fname.endsWith (".pls", Qt::CaseInsensitive))
             progLan = "config";
        else if (fname.endsWith (".js"))
            progLan = "javascript";
        else if (fname.endsWith (".qml"))
            progLan = "qml";
        else if (fname.endsWith (".log", Qt::CaseInsensitive))
            progLan = "log";
        else if (fname.endsWith (".php"))
            progLan = "php";
        else if (fname.endsWith (".url"))
             progLan = "url";
        else if (fname.endsWith (".diff") || fname.endsWith (".patch"))
            progLan = "diff";
        else if (fname.endsWith (".srt"))
            progLan = "srt";
        else if (fname.endsWith (".theme"))
             progLan = "theme";
        else if (fname.endsWith (".rc"))
             progLan = "gtkrc";
        else if (fname.endsWith (".m3u", Qt::CaseInsensitive))
            progLan = "m3u";
        else if (fname.endsWith (".htm", Qt::CaseInsensitive) || fname.endsWith (".html", Qt::CaseInsensitive))
            progLan = "html";
        else if (fname.endsWith (".markdown") || fname.endsWith (".md") || fname.endsWith (".mkd"))
            progLan = "markdown";
        else if (baseName == "sources.list" || baseName == "sources.list.save")
            progLan = "sourceslist";
        else if (baseName.startsWith ("makefile.", Qt::CaseInsensitive) && !baseName.endsWith (".txt", Qt::CaseInsensitive))
            progLan = "makefile";
        else if (baseName.compare ("CMakeLists.txt", Qt::CaseInsensitive) == 0)
            progLan = "cmake";
    }
    else if (baseName == "PKGBUILD")
        progLan = "sh";
    /* makefile is an exception */
    else if (baseName.compare ("makefile", Qt::CaseInsensitive) == 0)
        progLan = "makefile";
    else if (baseName.compare ("changelog", Qt::CaseInsensitive) == 0)
        progLan = "changelog";
    else if (baseName == "gtkrc")
        progLan = "gtkrc";
    else if (baseName == "control")
        progLan = "deb";

    if (progLan.isEmpty()) // now, check mime types
    {
        QString mime;
        if (!QFileInfo (fname).isSymLink())
            mime = getMimeType (fname);
        else
            mime = getMimeType (QFileInfo (fname).symLinkTarget());

        if (mime == "text/x-c++" || mime == "text/x-c++src" || mime == "text/x-c++hdr" || mime == "text/x-chdr")
            progLan = "cpp";
        else if (mime == "text/x-c" || mime == "text/x-csrc")
            progLan = "c";
        else if (mime == "application/x-shellscript" || mime == "text/x-shellscript")
            progLan = "sh";
        else if (mime == "application/x-ruby")
            progLan = "ruby";
        else if (mime == "text/x-lua")
            progLan = "lua";
        else if (mime == "text/x-python")
            progLan = "python";
        else if (mime == "application/x-perl")
            progLan = "perl";
        else if (mime == "text/x-makefile")
            progLan = "makefile";
        else if (mime == "text/x-cmake")
            progLan = "cmake";
        else if (mime == "application/vnd.nokia.qt.qmakeprofile")
            progLan = "qmake";
        else if (mime == "text/troff")
            progLan = "troff";
        else if (mime == "application/xml" || mime == "image/svg+xml" || mime == "application/x-designer"
                 || mime == "application/metalink4+xml" || mime == "application/metalink+xml"
                 || mime == "application/x-gtk-builder" || mime == "text/rdf+xml" || mime == "application/rdf+xml"
                 || mime == "application/x-docbook+xml" || mime == "application/x-xbel" || mime == "text/feathernotes-fnx"
                 || mime == "text/vnd.trolltech.linguist"
                 || mime == "application/xspf+xml" || mime == "audio/x-ms-asx")
            progLan = "xml";
        else if (mime == "text/css")
            progLan = "css";
        else if (mime == "text/x-changelog")
            progLan = "changelog";
        else if (mime == "application/x-desktop")
            progLan = "desktop";
        else if (mime == "audio/x-scpls")
            progLan = "config";
        else if (mime == "application/javascript")
            progLan = "javascript";
        else if (mime == "text/x-qml")
            progLan = "qml";
        else if (mime == "text/x-log")
            progLan = "log";
        else if (mime == "application/x-php" || mime == "text/x-php")
            progLan = "php";
        else if (mime == "application/x-mswinurl")
            progLan = "url";
        else if (mime == "application/x-theme")
            progLan = "theme";
        else if (mime == "text/x-diff" || mime == "text/x-patch")
            progLan = "diff";
        else if (mime == "text/html" || mime == "application/xhtml+xml")
            progLan = "html";
        else if (mime == "audio/x-mpegurl")
            progLan = "m3u";
        /* start checking contents */
        /*else
        {
            QString firstBlock = textEdit->document()->firstBlock().text();
            if (firstBlock.startsWith ("<html>")
                || firstBlock.startsWith ("<!DOCTYPE html"))
            {
                highlighter = new Highlighter (textEdit->document(), "html");
            }
        }*/
    }

    textEdit->setProg (progLan);
}
/*************************/
void FPwin::syntaxHighlighting (TextEdit *textEdit, bool highlight)
{
    if (textEdit == nullptr) return;

    if (highlight)
    {
        QString progLan = textEdit->getProg();
        if (progLan.isEmpty()
            || progLan == "help") // used for marking the help doc
        {
            return;
        }

        Config config = static_cast<FPsingleton*>(qApp)->getConfig();
        if (textEdit->getSize() > config.getMaxSHSize()*1024*1024)
            return;

        if (!qobject_cast< Highlighter *>(textEdit->getHighlighter()))
        {
            QPoint Point (0, 0);
            QTextCursor start = textEdit->cursorForPosition (Point);
            Point = QPoint (textEdit->geometry().width(), textEdit->geometry().height());
            QTextCursor end = textEdit->cursorForPosition (Point);

            textEdit->setDrawIndetLines (config.getShowWhiteSpace());
            Highlighter *highlighter = new Highlighter (textEdit->document(), progLan, start, end,
                                                        textEdit->hasDarkScheme(),
                                                        config.getShowWhiteSpace(),
                                                        config.getShowEndings());
            textEdit->setHighlighter (highlighter);

            QCoreApplication::processEvents(); // it's necessary to wait until the text is completely loaded
        }
        matchBrackets(); // in case the cursor is beside a bracket when the text is loaded
        connect (textEdit, &TextEdit::updateBracketMatching, this, &FPwin::matchBrackets);
        /* visible text may change on block removal */
        connect (textEdit, &QPlainTextEdit::blockCountChanged, this, &FPwin::formatOnBlockChange);
        connect (textEdit, &TextEdit::updateRect, this, &FPwin::formatVisibleText);
        connect (textEdit, &TextEdit::resized, this, &FPwin::formatOnResizing);
        /* this is needed when the whole visible text is pasted */
        connect (textEdit->document(), &QTextDocument::contentsChange, this, &FPwin::formatOnTextChange);
    }
    else if (Highlighter *highlighter = qobject_cast< Highlighter *>(textEdit->getHighlighter()))
    {
        disconnect (textEdit->document(), &QTextDocument::contentsChange, this, &FPwin::formatOnTextChange);
        disconnect (textEdit, &TextEdit::resized, this, &FPwin::formatOnResizing);
        disconnect (textEdit, &TextEdit::updateRect, this, &FPwin::formatVisibleText);
        disconnect (textEdit, &QPlainTextEdit::blockCountChanged, this, &FPwin::formatOnBlockChange);
        disconnect (textEdit, &TextEdit::updateBracketMatching, this, &FPwin::matchBrackets);

        /* remove bracket highlights */
        QList<QTextEdit::ExtraSelection> es = textEdit->extraSelections();
        int n = textEdit->getRedSel().count();
        while (n > 0 && !es.isEmpty())
        {
            es.removeLast();
            --n;
        }
        textEdit->setRedSel (QList<QTextEdit::ExtraSelection>());
        textEdit->setExtraSelections (es);

        textEdit->setDrawIndetLines (false);

        textEdit->setHighlighter (nullptr); // for consistency
        delete highlighter; highlighter = nullptr;
    }
}
/*************************/
void FPwin::formatOnTextChange (int /*position*/, int charsRemoved, int charsAdded) const
{
    if (charsRemoved > 0 || charsAdded > 0)
    {
        /* wait until the document's layout manager is notified about the change;
           otherwise, the end cursor might be out of range in formatTextRect() */
        QTimer::singleShot (0, [=]() {
            formatOnResizing();
        });
    }
}
/*************************/
void FPwin::formatOnBlockChange (int/* newBlockCount*/) const
{
    formatOnResizing();
}
/*************************/
void FPwin::formatVisibleText (const QRect &rect, int dy) const
{
    if (dy == 0) return;
    formatTextRect (rect);
}
/*************************/
void FPwin::formatOnResizing() const
{
    int index = ui->tabWidget->currentIndex();
    if (index == -1) return;

    TextEdit *textEdit = qobject_cast< TabPage *>(ui->tabWidget->widget (index))->textEdit();
    formatTextRect (textEdit->rect());
}
/*************************/
void FPwin::formatTextRect (QRect rect) const
{
    if (TabPage *tabPage = qobject_cast<TabPage*>(ui->tabWidget->currentWidget()))
    {
        TextEdit *textEdit = tabPage->textEdit();
        Highlighter *highlighter = qobject_cast< Highlighter *>(textEdit->getHighlighter());
        if (highlighter == nullptr) return;
        QString progLan = textEdit->getProg();
        if (progLan.isEmpty()) return;

        QPoint Point (0, 0);
        QTextCursor start = textEdit->cursorForPosition (Point);
        Point = QPoint (rect.width(), rect.height());
        QTextCursor end = textEdit->cursorForPosition (Point);

        highlighter->setLimit (start, end);
        QTextBlock block = start.block();
        while (block.isValid() && block.blockNumber() <= end.blockNumber())
        {
            if (TextBlockData *data = static_cast<TextBlockData *>(block.userData()))
            {
                if (!data->isHighlighted()) // isn't highlighted (completely)
                    highlighter->rehighlightBlock (block);
            }
            block = block.next();
        }
    }
}

}
