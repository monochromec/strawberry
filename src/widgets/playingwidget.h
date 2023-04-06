/*
 * Strawberry Music Player
 * This file was part of Clementine.
 * Copyright 2010, David Sansome <me@davidsansome.com>
 * Copyright 2013-2021, Jonas Kvinge <jonas@jkvinge.net>
 *
 * Strawberry is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Strawberry is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Strawberry.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PLAYINGWIDGET_H
#define PLAYINGWIDGET_H

#include <memory>

#include <QtGlobal>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <QSize>
#include <QAction>
#include <QMovie>

#include "core/song.h"
#include "covermanager/albumcoverloaderoptions.h"
#include "covermanager/albumcoverimageresult.h"

class QTimeLine;
class QTextDocument;
class QPainter;
class QMenu;
class QActionGroup;
class QContextMenuEvent;
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
class QPaintEvent;
class QResizeEvent;

class AlbumCoverChoiceController;
class Application;

class PlayingWidget : public QWidget {
  Q_OBJECT

 public:
  explicit PlayingWidget(QWidget *parent = nullptr);

  void Init(Application *app, AlbumCoverChoiceController *album_cover_choice_controller);
  bool IsEnabled() { return enabled_; }
  void SetEnabled(const bool enabled);
  void SetEnabled();
  void SetDisabled();
  void set_ideal_height(const int height);
  QSize sizeHint() const override;
  bool show_above_status_bar() const { return above_statusbar_action_->isChecked(); }

 signals:
  void ShowAboveStatusBarChanged(bool above);

 public slots:
  void Playing();
  void Stopped();
  void Error();
  void SongChanged(const Song &song);
  void SearchCoverInProgress();
  void AlbumCoverLoaded(const Song &song, AlbumCoverImageResultPtr result);

 protected:
  void paintEvent(QPaintEvent *e) override;
  void resizeEvent(QResizeEvent*) override;
  void contextMenuEvent(QContextMenuEvent *e) override;
  void mouseDoubleClickEvent(QMouseEvent*) override;
  void dragEnterEvent(QDragEnterEvent *e) override;
  void dropEvent(QDropEvent *e) override;

 private:
  enum class Mode {
    SmallSongDetails = 0,
    LargeSongDetails = 1
  };

 private slots:
  void Update() { update(); }
  void SetMode(const Mode mode);
  void ShowAboveStatusBar(const bool above);
  void FitCoverWidth(const bool fit);

  void AutomaticCoverSearchDone();

  void SetHeight(const int height);
  void FadePreviousTrack(const qreal value);

 private:
  static const char *kSettingsGroup;
  static const int kPadding;
  static const int kGradientHead;
  static const int kGradientTail;
  static const int kMaxCoverSize;
  static const int kBottomOffset;
  static const int kTopBorder;

  Application *app_;
  AlbumCoverChoiceController *album_cover_choice_controller_;
  Mode mode_;
  QMenu *menu_;
  QAction *above_statusbar_action_;
  QAction *fit_cover_width_action_;
  bool enabled_;
  bool visible_;
  bool playing_;
  bool active_;
  int small_ideal_height_;
  AlbumCoverLoaderOptions cover_loader_options_;
  int total_height_;
  bool fit_width_;
  QTimeLine *timeline_show_hide_;
  QTimeLine *timeline_fade_;
  QTextDocument *details_;
  qreal pixmap_previous_track_opacity_;
  bool downloading_covers_;

  Song song_;
  Song song_playing_;
  QImage image_current_;
  QImage image_original_;
  QPixmap pixmap_cover_;
  QPixmap pixmap_previous_track_;
  std::unique_ptr<QMovie> spinner_animation_;

  void SetVisible(const bool visible);
  void CreateModeAction(const Mode mode, const QString &text, QActionGroup *group);
  void UpdateDetailsText();
  void UpdateHeight();
  void SetImage(const QImage &image = QImage());
  void DrawContents(QPainter *p);
  void ScaleCover();
  void GetCoverAutomatically();

};

#endif  // PLAYINGWIDGET_H
