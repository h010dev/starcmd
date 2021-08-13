/*
 * Starcmd Command Browser
 *
 * Copyright © 2021 Mohamed Al-Hussein <mohamed.n.al.hussein@gmail.com>
 * 
 * SPDX-License-Identifier: GPL-3.0-only
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <curl/curl.h>

#include "starcmd-image-client.h"

static size_t
write_data (void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite (ptr, size, nmemb, (FILE *) stream);
    return written;
}

static int
download_file (const char *url, const char *f_name)
{
    CURL *curl_handle;
    FILE *f_out;

    curl_global_init (CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init ();
    curl_easy_setopt (curl_handle, CURLOPT_URL, url);
    curl_easy_setopt (curl_handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt (curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt (curl_handle, CURLOPT_WRITEFUNCTION, write_data);

    f_out = fopen (f_name, "wb");

    if (f_out)
    {
        curl_easy_setopt (curl_handle, CURLOPT_WRITEDATA, f_out);
        curl_easy_perform (curl_handle);
        fclose (f_out);
    }

    curl_easy_cleanup (curl_handle);
    curl_global_cleanup ();

    return 0;
}

static int
get_image_url (char *name, char **url)
{
    char *api; 
    char *api_url;
    char *meta_dir;
    char *f_name;
    char *f_type;
    char *f_dir;

    // Append name to api url
    api = "https://cs361service.herokuapp.com/";
    api_url = malloc (strlen (api) + strlen (name) + 1);
    api_url[0] = '\0';
    strcat (api_url, api);
    strcat (api_url, name);

    // Download json file from api with image url inside
    meta_dir = "../img/meta/";
    f_name = strdup (name);
    f_type = ".json";
    f_dir = malloc (strlen (meta_dir) + strlen (f_name) + strlen (f_type) + 1);

    f_dir[0] = '\0';
    strcat (f_dir, meta_dir);
    strcat (f_dir, f_name);
    strcat (f_dir, f_type);
    download_file (api_url, f_dir);

    // Open downloaded json file
    FILE *fp;
    if ( (fp = fopen (f_dir, "rb")) == NULL)
        return 1;

    char *f_data;
    long  f_size;

    // Allocate buffer for json file
    fseek (fp, 0L, SEEK_END);
    f_size = ftell (fp);
    rewind (fp);

    if ( (f_data = calloc (1, f_size + 1)) == NULL)
    {
        fclose (fp);
        free (f_data);
        return 1;
    }

    // Read file into buffer
    if (fread (f_data, f_size, 1, fp) != 1)
    {
        fclose (fp);
        free (f_data);
        return 1;
    }
    fclose (fp);

    // Seek to image url
    char *cur, *cur_end, *end_match, *s_url, *url_match;
    int pos;

    url_match = ":";

    cur = strstr (f_data, url_match);
    cur += strlen (url_match);
    cur++;
    end_match = "\"";
    cur_end = strstr (cur, end_match);

    // Allocate space for image url
    if ( (s_url = calloc (1, (char *) cur_end - (char *) cur + 1)) == NULL)
    {
        free (f_data);
        return 1;
    }

    // Extract image url
    pos = 0;
    while (cur < cur_end)
    {
        s_url[pos] = cur[0];
        cur++;
        pos++;
    }

    *url = s_url;

    return 0;
}

int
download_image (char *name, char **img_path)
{
    char *url;
    char *img_dir;
    char *f_name;
    char *f_type;
    char *f_dir;

    // Get image url
    get_image_url (name, &url);

    // Download image
    img_dir = "../img/";
    f_name = strdup (name);
    f_type = ".png";
    f_dir = malloc (strlen (img_dir) + strlen (f_name) + strlen (f_type) + 1);
    f_dir[0] = '\0';

    strcat (f_dir, img_dir);
    strcat (f_dir, f_name);
    strcat (f_dir, f_type);

    download_file (url, f_dir);

    *img_path = f_dir;

    return 0;
}
