/*
 * Strawberry Music Player
 * This file was part of Clementine.
 * Copyright 2010, David Sansome <me@davidsansome.com>
 * Copyright 2018-2021, Jonas Kvinge <jonas@jkvinge.net>
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

#ifndef ALBUMCOVERMANAGER_H
#define ALBUMCOVERMANAGER_H

#include "config.h"

#include <QtGlobal>
#include <QMainWindow>
#include <QObject>
#include <QAbstractItemModel>
#include <QList>
#include <QListWidgetItem>
#include <QMap>
#include <QMultiMap>
#include <QString>
#include <QImage>
#include <QIcon>

#include "core/song.h"
#include "albumcoverloaderoptions.h"
#include "albumcoverloaderresult.h"
#include "albumcoverchoicecontroller.h"
#include "coversearchstatistics.h"
#include "settings/collectionsettingspage.h"

class QWidget;
class QMimeData;
class QMenu;
class QAction;
class QProgressBar;
class QPushButton;
class QEvent;
class QCloseEvent;
class QShowEvent;

class Application;
class CollectionBackend;
class SongMimeData;
class AlbumCoverExport;
class AlbumCoverExporter;
class AlbumCoverFetcher;
class AlbumCoverSearcher;

class Ui_CoverManager;

class AlbumItem : public QListWidgetItem {
 public:
  AlbumItem(const QIcon &icon, const QString &text, QListWidget *parent = nullptr, int type = Type) : QListWidgetItem(icon, text, parent, type) {};
  QList<QUrl> urls;

 private:
  Q_DISABLE_COPY(AlbumItem)
};

class AlbumCoverManager : public QMainWindow {
  Q_OBJECT

 public:
  explicit AlbumCoverManager(Application *app, CollectionBackend *collection_backend, QMainWindow *mainwindow, QWidget *parent = nullptr);
  ~AlbumCoverManager() override;

  static const char *kSettingsGroup;

  void Reset();
  void Init();

  void EnableCoversButtons();
  void DisableCoversButtons();

  SongList GetSongsInAlbum(const QModelIndex &idx) const;

  CollectionBackend *backend() const { return collection_backend_; }

 protected:
  void showEvent(QShowEvent *e) override;
  void closeEvent(QCloseEvent *e) override;

  // For the album view context menu events
  bool eventFilter(QObject *obj, QEvent *e) override;

 private:
  enum ArtistItemType {
    All_Artists,
    Various_Artists,
    Specific_Artist
  };

  enum Role {
    Role_AlbumArtist = Qt::UserRole + 1,
    Role_Album,
    Role_PathAutomatic,
    Role_PathManual,
    Role_Filetype,
    Role_CuePath,
    Role_ImageData,
    Role_Image
  };

  enum class HideCovers {
    None,
    WithCovers,
    WithoutCovers
  };

  void LoadGeometry();
  void SaveSettings();

  QString InitialPathForOpenCoverDialog(const QString &path_automatic, const QString &first_file_name) const;

  // Returns the selected element in form of a Song ready to be used by AlbumCoverChoiceController or invalid song if there's nothing or multiple elements selected.
  Song GetSingleSelectionAsSong();
  // Returns the first of the selected elements in form of a Song ready to be used by AlbumCoverChoiceController or invalid song if there's nothing selected.
  Song GetFirstSelectedAsSong();

  Song ItemAsSong(QListWidgetItem *item) { return ItemAsSong(static_cast<AlbumItem*>(item)); }
  static Song ItemAsSong(AlbumItem *item);

  void UpdateStatusText();
  bool ShouldHide(const AlbumItem &item, const QString &filter, const HideCovers hide_covers) const;
  void SaveAndSetCover(AlbumItem *item, const AlbumCoverImageResult &result);

  void SaveImageToAlbums(Song *song, const AlbumCoverImageResult &result);

  SongList GetSongsInAlbums(const QModelIndexList &indexes) const;
  SongMimeData *GetMimeDataForAlbums(const QModelIndexList &indexes) const;

  bool ItemHasCover(const AlbumItem &item) const;

 signals:
  void Error(const QString &error);
  void AddToPlaylist(QMimeData *data);

 private slots:
  void ArtistChanged(QListWidgetItem *current);
  void AlbumCoverLoaded(const quint64 id, const AlbumCoverLoaderResult &result);
  void UpdateFilter();
  void FetchAlbumCovers();
  void ExportCovers();
  void AlbumCoverFetched(const quint64 id, const AlbumCoverImageResult &result, const CoverSearchStatistics &statistics);
  void CancelRequests();

  // On the context menu
  void FetchSingleCover();

  void LoadCoverFromFile();
  void SaveCoverToFile();
  void LoadCoverFromURL();
  void SearchForCover();
  void UnsetCover();
  void ClearCover();
  void DeleteCover();
  void ShowCover();

  // For adding albums to the playlist
  void AlbumDoubleClicked(const QModelIndex &idx);
  void AddSelectedToPlaylist();
  void LoadSelectedToPlaylist();

  void UpdateCoverInList(AlbumItem *item, const QUrl &cover);
  void UpdateExportStatus(const int exported, const int skipped, const int max);

  void SaveEmbeddedCoverAsyncFinished(quint64 id, const bool success);

 private:
  Ui_CoverManager *ui_;
  QMainWindow *mainwindow_;
  Application *app_;
  CollectionBackend *collection_backend_;
  AlbumCoverChoiceController *album_cover_choice_controller_;

  QAction *filter_all_;
  QAction *filter_with_covers_;
  QAction *filter_without_covers_;

  AlbumCoverLoaderOptions cover_loader_options_;
  QMap<quint64, AlbumItem*> cover_loading_tasks_;

  AlbumCoverFetcher *cover_fetcher_;
  QMap<quint64, AlbumItem*> cover_fetching_tasks_;
  CoverSearchStatistics fetch_statistics_;

  AlbumCoverSearcher *cover_searcher_;
  AlbumCoverExport *cover_export_;
  AlbumCoverExporter *cover_exporter_;

  QIcon artist_icon_;
  QIcon all_artists_icon_;
  const QImage image_nocover_thumbnail_;
  const QIcon icon_nocover_item_;

  QMenu *context_menu_;
  QList<QListWidgetItem*> context_menu_items_;

  QProgressBar *progress_bar_;
  QPushButton *abort_progress_;
  int jobs_;

  QMultiMap<quint64, AlbumItem*> cover_save_tasks_;
  QList<AlbumItem*> cover_save_tasks2_;

  QListWidgetItem *all_artists_;

};

#endif  // ALBUMCOVERMANAGER_H
