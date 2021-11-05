FROM ubuntu:focal

MAINTAINER Mathew Benson <mbenson@windhoverlabs.com>

RUN apt-get update && apt-get install -y --no-install-recommends apt-utils
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends dialog apt-utils
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
RUN apt-get install -y -q
RUN apt-get install -y openjdk-8-jdk
RUN apt-get install -y default-jre
RUN apt-get install -y supervisor
RUN apt-get install -y apache2
#RUN apt-get install maven

COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf

CMD ["/usr/bin/supervisord"]