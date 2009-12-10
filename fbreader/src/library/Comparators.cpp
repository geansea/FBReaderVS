/*
 * Copyright (C) 2009 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "Book.h"
#include "Author.h"
#include "Tag.h"
#include "Comparators.h"

bool BookComparator::operator() (
	const shared_ptr<Book> book0,
	const shared_ptr<Book> book1
) {
	const std::string &seriesName0 = book0->seriesName();
	const std::string &seriesName1 = book1->seriesName();
	int comp = seriesName0.compare(seriesName1);
	if (comp == 0) {
		if (!seriesName0.empty()) {
			comp = book0->indexInSeries() - book1->indexInSeries();
			if (comp != 0) {
				return comp < 0;
			}
		}
		return book0->title() < book1->title();
	}
	if (seriesName0.empty()) {
		return book0->title() < seriesName1;
	}
	if (seriesName1.empty()) {
		return seriesName0 <= book1->title();
	}
	return comp < 0;
}

bool AuthorComparator::operator() (
	const shared_ptr<Author> author0,
	const shared_ptr<Author> author1
) {
	if (author0.isNull()) {
		return !author1.isNull();
	}
	if (author1.isNull()) {
		return false;
	}

	const int comp = author0->sortKey().compare(author1->sortKey());
	return comp != 0 ? comp < 0 : author0->name() < author1->name();
}

bool TagComparator::operator() (
	shared_ptr<Tag> tag0,
	shared_ptr<Tag> tag1
) {
	if (tag0.isNull()) {
		return !tag1.isNull();
	}
	if (tag1.isNull()) {
		return false;
	}

	size_t level0 = tag0->level();
	size_t level1 = tag1->level();
	if (level0 > level1) {
		for (; level0 > level1; --level0) {
			tag0 = tag0->parent();
		}
		if (tag0 == tag1) {
			return false;
		}
	} else if (level0 < level1) {
		for (; level0 < level1; --level1) {
			tag1 = tag1->parent();
		}
		if (tag0 == tag1) {
			return true;
		}
	}
	while (tag0->parent() != tag1->parent()) {
		tag0 = tag0->parent();
		tag1 = tag1->parent();
	}
	return tag0->name() < tag1->name();
}
