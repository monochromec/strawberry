/*
 * Strawberry Music Player
 * Copyright 2020-2021, Jonas Kvinge <jonas@jkvinge.net>
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

#ifndef ALBUMCOVERLOADERRESULT_H
#define ALBUMCOVERLOADERRESULT_H

#include "config.h"

#include <QImage>
#include <QUrl>

#include "albumcoverimageresult.h"

class AlbumCoverLoaderResult {
 public:

  enum class Type {
    None,
    ManuallyUnset,
    Embedded,
    Automatic,
    Manual,
    Remote
  };

  explicit AlbumCoverLoaderResult(const bool _success = false,
                                  const Type _type = Type::None,
                                  AlbumCoverImageResult _album_cover = AlbumCoverImageResult(),
                                  const QImage &_image_scaled = QImage(),
                                  const QImage &_image_thumbnail = QImage(),
                                  const bool _updated = false) :
                                  success(_success),
                                  type(_type),
                                  album_cover(_album_cover),
                                  image_scaled(_image_scaled),
                                  image_thumbnail(_image_thumbnail),
                                  updated(_updated) {}

  bool success;
  Type type;
  AlbumCoverImageResult album_cover;
  QImage image_scaled;
  QImage image_thumbnail;
  bool updated;

  QUrl temp_cover_url;

};

Q_DECLARE_METATYPE(AlbumCoverLoaderResult)

#endif  // ALBUMCOVERLOADERRESULT_H
